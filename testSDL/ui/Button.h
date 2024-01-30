#pragma once
#include "TextBox.h"
#include "../Timer.h"


namespace ui
{
	class Button : public TextBox
	{
	protected:
		bool mClicked = false;
		Milliseconds mReclickCooldown = 0, mReclickTime = 0;
		const Color mClickedTint = DARK_GREY;
	public:
		Button(Widget* parent, SDL_FRect area,
			const std::string& name, int textureID,
			WidgetPostion x = WIDGET_LEFT, WidgetPostion y = WIDGET_TOP,
			Milliseconds cooldown = 500);

		Button* SetCooldown(Milliseconds ms);

		void Update() override;
		void Draw() override;
	};

}