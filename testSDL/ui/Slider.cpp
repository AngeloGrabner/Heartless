#include "Slider.h"
#include "../Input.h"
#include "../TextureManager.h"
#include <algorithm>

ui::Slider::Slider(Widget* parent, SDL_FRect area, const std::string& name, bool vertical, WidgetPostion x, WidgetPostion y, int SliderTexID,  SDL_FPoint sliderButtonSize, float initalValue)
	: Widget(parent,area,name,x,y), mTexID(SliderTexID), mVert(vertical)
{
	float X = mArea.x + mArea.w / 2.0f - sliderButtonSize.x / 2.0f,
		Y = mArea.y + mArea.h / 2.0f - sliderButtonSize.y / 2.0f;
	if (!mVert)
		X = lerp(mArea.x, mArea.x + mArea.w - sliderButtonSize.x, std::clamp(initalValue, 0.0f, 1.0f));
	else
		Y = lerp(mArea.y, mArea.y + mArea.h - sliderButtonSize.y, std::clamp(initalValue, 0.0f, 1.0f));

	mSliderArea = { X,Y,sliderButtonSize.x,sliderButtonSize.y };
}

ui::Slider* ui::Slider::SetTexture(int texId)
{
	mTexID = texId;
	return this;
}

ui::Slider* ui::Slider::SetSliderButtionSize(SDL_FPoint size)
{
	float X = mArea.x + mArea.w / 2.0f - size.x / 2.0f,
		Y = mArea.y + mArea.h / 2.0f - size.y / 2.0f;
	if (!mVert)
		X = lerp(mArea.x, mArea.x + mArea.w - size.x, std::clamp(mValue, 0.0f, 1.0f));
	else
		Y = lerp(mArea.y, mArea.y + mArea.h - size.y, std::clamp(mValue, 0.0f, 1.0f));

	mSliderArea = { X,Y,size.x,size.y };
	return this;
}

ui::Slider* ui::Slider::SetValue(float normalized)
{
	mValue = std::clamp(normalized, 0.0f, 1.0f);
	return this;
}

ui::Slider* ui::Slider::SetSnappiness(bool snap, int steps)
{
	mSnap = snap;
	mSnapSteps = steps -1 ;
	return this;
}

void ui::Slider::Update()
{
	if (!mActive)
		return;
	Widget::Update();
	UpdateChildrin();
	if (mHeld)
	{
		if (mVert)
		{	
			mSliderArea.y += Input::GetRelativeMousePos().y;
			mSliderArea.y = std::clamp(mSliderArea.y, mArea.y, mArea.y + mArea.h - mSliderArea.h);
		}
		else
		{
			mSliderArea.x += Input::GetRelativeMousePos().x;
			mSliderArea.x = std::clamp(mSliderArea.x, mArea.x, mArea.x + mArea.w - mSliderArea.w);
		}

	}
	if (contain(mSliderArea,Input::GetMousePos()) && Input::GetMouse(Input::LMB, Input::UI).Down())
	{
		//DB_OUT((int)Input::INTERN);
		Input::Handled(Input::LMB, LAYER_UI);
		mHeld = true;
	}
	else if (mHeld && Input::GetMouse(Input::LMB, LAYER_UI).Up())
	{
		if (mVert)
		{
			mValue = invlerp(mArea.y, mArea.y + mArea.h - mSliderArea.h, mSliderArea.y);
			if (mSnap)
			{
				auto temp = mValue * mSnapSteps;
				temp = std::round(temp);
				temp /= (float)mSnapSteps;
				mValue = temp; 
				mSliderArea.y = lerp(mArea.y, mArea.y + mArea.h - mSliderArea.h, mValue);
			}
		}
		else
		{
			mValue = invlerp(mArea.x, mArea.x + mArea.w - mSliderArea.w, mSliderArea.x);
			if (mSnap)
			{
				auto temp = mValue * mSnapSteps;
				temp = std::round(temp);
				temp /= (float)mSnapSteps;
				mValue = temp;
				mSliderArea.x = lerp(mArea.x, mArea.x + mArea.w - mSliderArea.w, mValue);
			}
		}
		mHeld = false;
		EventBuilder::Slider(mName,mValue);
	}
}

void ui::Slider::Draw()
{
	if (!mActive)
		return;
	auto tex = TextureManager::Get(mTexID);
	Renderer::SetColor(WHITE);
	if (mVert)
		Renderer::DrawLine(SDL_FPoint{mSliderArea.x + mSliderArea.w/2,mArea.y + 5}, SDL_FPoint{ mSliderArea.x + mSliderArea.w / 2,mArea.y +mArea.h - 10});
	else
		Renderer::DrawLine(SDL_FPoint{ mArea.x + 5, mSliderArea.y + mSliderArea.h / 2 }, SDL_FPoint{mArea.x + mArea.w - 10, mSliderArea.y + mSliderArea.h / 2});
	if (mHeld)
		tex.SetTint(mTint);
	Renderer::DrawTexture(tex, mSliderArea);
	tex.SetTint(WHITE);
	DrawChildrin();
}

void ui::Slider::DebugDraw() 
{
	bool flag = mChildrinDebugDrawFlag;
	mChildrinDebugDrawFlag = true;
	Widget::DebugDraw();
	Renderer::SetColor(MAGENTA);
	Renderer::DrawRect(mSliderArea);
	if (flag)
		mChildrinDebugDrawFlag = false;
	DebugDrawChildrin();
}
