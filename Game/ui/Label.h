#pragma once
#include "Ui.h"

namespace ui
{
	// displayed text size is based on mArea.h 
	class Label : public Widget
	{
	protected:
		std::string mText;
		Color mTextColor = WHITE;
		SDL_FPoint mTextScale = { 1.0f, 1.0f };
		int mFontId = -1;
		WidgetPostion mAlignment = WidgetPostion::WIDGET_CENTER;
	public:
		Label(Widget* parent,
			SDL_FRect area,
			const std::string& name,
			WidgetPostion x = WIDGET_LEFT,
			WidgetPostion y = WIDGET_TOP,
			WidgetPostion HorizontalTextAlignment = WIDGET_CENTER,
			const std::string& text = "");

		void Handle(const SDL_Event* e) override;
		void Draw() override;

		Label* SetAlignment(WidgetPostion horizontal);
		Label* SetFont(int fontId);
		Label* SetText(const std::string& text, Color c = WHITE);
	};
}