#include "Label.h"
#include "Font.h"

ui::Label::Label(Widget* parent, SDL_FRect area, const std::string& name, WidgetPostion x, WidgetPostion y, WidgetPostion HorizontalTextAlignment, const std::string& text)
	: Widget(parent,area,name,x,y), mText(text)
{
	mTextScale.y = mArea.h/FontWriter::GetSize().y;
	mTextScale.x = mTextScale.y;
}

void ui::Label::Handle(const SDL_Event* e)
{
	Widget::Handle(e);
	std::string name, text;
	if (EventReceiver::ReLabel(e, name, text))
	{
		if (name == mName)
			mText = text;
	}
}

void ui::Label::Draw()
{
	if (!mActive)
		return;

	int tempFontId = FontWriter::GetFont();
	if (mFontId != -1)
		FontWriter::SetFont(mFontId);
	float textWidth = FontWriter::GetStrSize(mText,mTextScale).x;

	auto innerArea = GetInnerArea();

	SDL_assert(textWidth <= innerArea.w); // not enought space -> textWidth - innerArea.w needed

	SDL_Rect area = cast<SDL_FRect, SDL_Rect>(innerArea);
	if (mAlignment == WIDGET_CENTER)
	{
		area.x += innerArea.w / 2 - textWidth / 2;
	}
	else if (mAlignment == WIDGET_RIGHT)
	{
		area.x += (innerArea.w - textWidth);
	}

	FontWriter::DrawText(area,mText,mTextScale,mTextColor,true);

	FontWriter::SetFont(tempFontId);
	DrawChildrin();
}

ui::Label* ui::Label::SetAlignment(WidgetPostion horizontal)
{
	mAlignment = horizontal;
	return this;
}

ui::Label* ui::Label::SetFont(int fontId)
{
	mFontId = fontId;

	auto temp = FontWriter::GetFont();

	FontWriter::SetFont(mFontId);

	mTextScale.y = GetInnerArea().y / FontWriter::GetSize().y;
	mTextScale.x = mTextScale.y;

	FontWriter::SetFont(temp);
	return this;
}

ui::Label* ui::Label::SetText(const std::string& text, Color c)
{
	mTextColor = c;
	mText = text;
	return this;
}
