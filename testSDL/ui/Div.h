#pragma once
#include "Ui.h"

namespace ui
{
	class Div final :  public Widget // just a div like in html
	{
	public:
		Div(Widget* parent, SDL_FRect area, WidgetPostion x = WIDGET_LEFT, WidgetPostion y = WIDGET_TOP);
		void Update() override;
		void Draw() override;
	};
}