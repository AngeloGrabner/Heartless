#include "Button.h"

#include "../Input.h"

namespace ui
{

	Button::Button(Widget* parent, SDL_FRect area, const std::string& name, int textureID, WidgetPostion x, WidgetPostion y, Milliseconds cooldown)
		: TextBox(parent,area,name,textureID,x,y), mReclickCooldown(cooldown)
	{

	}
	Button* Button::SetCooldown(Milliseconds ms)
	{
		mReclickCooldown = ms;
		return this;
	}
	void Button::Update()
	{
		if (!mActive)
			return;
		TextBox::Update();
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

		bool flag = mChildrinDrawFlag;
		mChildrinDrawFlag = true;
		TextBox::Draw();
		mTextColor = temp;
		if (!flag)
			mChildrinDrawFlag = false;
		DrawChildrin();
	}
}