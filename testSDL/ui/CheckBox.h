#pragma once
#include "Ui.h"

namespace ui
{
	class CheckBox : public Widget
	{
	protected:
		int mCheckTexId = -1;
		int mBoxTexId = -1;
		bool mChecked = false;
		const Color mTint = GREY;
	public:
		CheckBox(Widget* parent, SDL_FRect area, const std::string& name, WidgetPostion x = WIDGET_LEFT, WidgetPostion y = WIDGET_TOP, int boxTextureId = -1, int checkTextureId = -1, bool checked = false);
		
		CheckBox* SetTextures(int boxTextureId, int checkTextureId);
		CheckBox* SetChecked(bool checked);

		void Handle(const SDL_Event* e) override;
		void Update() override;
		void Draw() override;
	};
}