#pragma once
#include "Label.h"
#include "../Timer.h"


namespace ui
{
	class Button : public Label
	{
	protected:
		bool mClicked = false;
		int mTexId = -1;
		Milliseconds mReclickCooldown = 0, mReclickTime = 0;
		const Color mClickedTint = DARK_GREY;
		Color mBackgroundTint = WHITE; 
	public:
		Button(Widget* parent, SDL_FRect area,
			const std::string& name, int textureID,
			WidgetPostion x = WIDGET_LEFT, WidgetPostion y = WIDGET_TOP,
			Milliseconds cooldown = 500);

		Button* SetCooldown(Milliseconds ms);
		Button* SetTexture(int textureId);
		Button* SetText(const std::string& text);

		void Update() override;
		void Draw() override;
	};

}