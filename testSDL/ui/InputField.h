#pragma once
#include "Ui.h"

namespace ui
{
	class InputField : public Widget
	{
	protected:
		std::string mText;
		int mTexId = -1;
		int mFontId = -1;
		unsigned int mCursor = 0;
		bool mSelected = false;
		Color mTextTint = WHITE;
	public:
		InputField(Widget* parent, SDL_FRect area,
			const std::string& name, WidgetPostion x = WIDGET_LEFT,
			WidgetPostion y = WIDGET_TOP,
			int textureID = -1, int fontID = -1, 
			const std::string& defaultText = "");
	
		InputField* SetTexture(int textureId);
		InputField* SetText(const std::string& defaultText);
		InputField* SetFont(int fontId);

		void Update() override;
		void Draw() override;
	};
}