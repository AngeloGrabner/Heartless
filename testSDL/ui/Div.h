#pragma once
#include "Ui.h"

namespace ui
{
	class Div final :  public Widget // just a div like in html
	{
	public:
		Div(Widget* parent, SDL_FRect area,
			WidgetPostion x = WIDGET_LEFT,
			WidgetPostion y = WIDGET_TOP,
			const std::string& name = "DIV");
		Div* SetName(const std::string name);

		void Update() override;
		void Draw() override;
	};
}