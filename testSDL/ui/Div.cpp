#include "Div.h"

ui::Div::Div(Widget* parent, SDL_FRect area, WidgetPostion x, WidgetPostion y, const std::string& name)
	: Widget(parent,area,name,x,y)
{
}

ui::Div* ui::Div::SetName(const std::string name)
{
	mName = name;
	return this;
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
