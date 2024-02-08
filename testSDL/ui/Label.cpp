#include "Label.h"
#include "Font.h"

ui::Label::Label(Widget* parent, SDL_FRect area, const std::string& name, WidgetPostion x, WidgetPostion y, WidgetPostion HorizontalTextAlignment, const std::string& text)
	: Widget(parent,area,name,x,y), mText(text)
{
	mTextScale.y = mArea.h/FontWriter::GetSize().y;
	mTextScale.x = mTextScale.y;
}

void ui::Label::Draw()
{
	if (!mActive)
		return;

	int tempFontId = FontWriter::GetFont();
	if (mFontId != -1)
		FontWriter::SetFont(mFontId);
	float textWidth = 0;
	for (int i = 0; i < mText.size(); i++)
	{
		textWidth += FontWriter::GetSize(mText[i]).x;
	}
	textWidth *= mTextScale.x;

	auto innerArea = GetInnerArea();

#ifdef _DEBUG
	if (textWidth > innerArea.w)
	{
		throw std::exception(("at least " + std::to_string(textWidth - innerArea.w) + " pixels needed").c_str());
	}
#endif // _DEBUG

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