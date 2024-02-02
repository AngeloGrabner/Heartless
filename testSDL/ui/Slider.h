#pragma once
#include "Ui.h"

namespace ui
{
	// Slider event Values: top left is 0, bottom right is 1
	class Slider final : public Widget
	{
	protected:
		SDL_FRect mSliderArea = { 0,0,0,0 };
		int mTexID = -1; 
		bool mHeld = false;
		bool mVert = true;
		bool mSnap = false;
		int mSnapSteps = 1;
		float mValue = 0.0f;
		Color mTint = GREY;
	public:
		Slider(Widget* parent, SDL_FRect area, 
			const std::string& name, bool vertical, WidgetPostion x = WIDGET_LEFT,
			WidgetPostion y = WIDGET_TOP, int SliderTexID  = -1, 
			SDL_FPoint sliderButtonSize = {32,32},
			float initalValue = 0.0f);

		Slider* SetTexture(int texId);
		Slider* SetSliderButtionSize(SDL_FPoint size);
		Slider* SetValue(float normalized);
		Slider* SetSnappiness(bool snap, int steps);

		void Update() override;
		void Draw() override;
		void DebugDraw() override;
	};
}