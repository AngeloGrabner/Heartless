#include "Button.h"

#include "../TextureManager.h"
#include "../Input.h"

namespace ui
{

	Button::Button(Widget* parent, SDL_FRect area, const std::string& name, int textureID, WidgetPostion x, WidgetPostion y, Milliseconds cooldown)
		: Label(parent,area,name,x,y),mTexId(textureID), mReclickCooldown(cooldown)
	{

	}
	Button* Button::SetCooldown(Milliseconds ms)
	{
		mReclickCooldown = ms;
		return this;
	}
	Button* Button::SetTexture(int textureId)
	{
		mTexId = textureId;
		return this;
	}
	Button* Button::SetText(const std::string& text)
	{
		mText = text;
		return this;
	}
	void Button::Update()
	{
		if (!mActive)
			return;
		Widget::Update();
		UpdateChildrin();
		if (mClicked)
		{
			mReclickTime += Timer::GetGlobalDT();
			if (mReclickTime >= mReclickCooldown)
			{
				mBackgroundTint = WHITE;
				mClicked = false;
				mReclickTime = 0;
			}
		}
		if (contain(mArea, Input::GetMousePos()) && Input::GetMouse(Input::LMB,LAYER_UI).Down() && !mClicked)
		{
			Input::Handled(Input::LMB, LAYER_UI);
			mBackgroundTint = mClickedTint;
			mClicked = true;
			EventBuilder::Button(mName);
		}
	}
	void Button::Draw()
	{
		if (!mActive)
			return;
		Color temp = mTextColor;
		if (mClicked)
		{
			mTextColor.r *= mClickedTint.r / 255.0f;
			mTextColor.g *= mClickedTint.g / 255.0f;
			mTextColor.b *= mClickedTint.b / 255.0f;
			mTextColor.a *= mClickedTint.a / 255.0f;
		}

		auto tex = TextureManager::Get(mTexId);

		tex.SetTint(mBackgroundTint);

		tex.rect.w /= 3;
		SDL_FRect area = mArea;
		float yScale = area.h / tex.rect.h;
		{ // draw the box 
			area.w = tex.rect.w * yScale;
			Renderer::DrawTexture(tex, area);

			tex.rect.x += tex.rect.w;
			area.x += area.w;
			area.w = mArea.w - area.w * 2.0f;
			Renderer::DrawTexture(tex, area);

			tex.rect.x += tex.rect.w;
			area.w = tex.rect.w * yScale;
			area.x = mArea.x + mArea.w - area.w;
			Renderer::DrawTexture(tex, area);
		}

		tex.SetTint(WHITE);

		bool flag = mChildrinDrawFlag;
		mChildrinDrawFlag = true;
		Label::Draw();
		mTextColor = temp;
		if (!flag)
			mChildrinDrawFlag = false;
		DrawChildrin();
	}
}