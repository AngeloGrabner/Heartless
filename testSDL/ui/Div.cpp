#include "Div.h"

ui::Div::Div(Widget* parent, SDL_FRect area, WidgetPostion x, WidgetPostion y)
	: Widget(parent,area,"DIV",x,y)
{
}

void ui::Div::Update()
{
	if (!mActive)
		return;
	UpdateChildrin();
}

void ui::Div::Draw()
{
	if (!mActive)
		return;
	mChildrinDrawFlag = false;
	DrawChildrin();
}
