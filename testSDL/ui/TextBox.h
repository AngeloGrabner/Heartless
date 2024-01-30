#pragma once
#include "Frame.h"
#include "Font.h"
namespace ui
{
	class TextBox : public Frame
	{
	protected:
		std::string mText;
		Color mTextColor = WHITE;
		float mTextScale = 1.0f;
	public:
		TextBox(Widget* parent, SDL_FRect area,
			const std::string& name, int textureID,
			WidgetPostion x = WIDGET_LEFT, WidgetPostion y = WIDGET_TOP,
			const std::string& text = "", Color textColor = WHITE, 
			float TextScale = 1.0f);

		void Draw() override;
		void DebugDraw() override;

		TextBox* SetBackground(int texuteId, float scale);
		TextBox* SetText(const std::string& text, Color c = WHITE, float scale = 1.0f);
	};
}