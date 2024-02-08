#pragma once
#include "Ui.h"

#define TRANS_GREY Color(DARK_GREY.r, DARK_GREY.g, DARK_GREY.b, 255 / 3)

namespace ui
{ 
	class Select : public Widget
	{
		std::vector<std::string> mOptions;
		Color mTextColor;
		Color mSelectionBackgroundColor;
		WidgetPostion mAlignment; // todo not implemented
		int mTexId = -1;
		bool mShowOptions = false;
		float mTextScale = 1.0f;
		int mIndex = 0;
	public:
		Select(Widget* parent,
			SDL_FRect area,
			const std::string& name,
			int textureId,
			WidgetPostion x = WIDGET_LEFT,
			WidgetPostion y = WIDGET_TOP, 
			const std::vector<std::string>& options = {},
			Color textColor = WHITE,
			Color selectionBackgroundColor = TRANS_GREY,
			WidgetPostion alignment = WIDGET_LEFT);

		Select(Select&& other) noexcept;
		Select& operator=(Select&& other) noexcept;

		void Update() override;
		void Draw() override;
		//void DebugDraw() override;

		Select* SetColor(Color textColor, Color selectionBackgroundColor = TRANS_GREY);
		Select* SetOptions(const std::vector<std::string>& options);
		Select* AddOption(const std::string& option);
		Select* SetAlignment(WidgetPostion alignment);

	};
}
#undef TRANS_GREY