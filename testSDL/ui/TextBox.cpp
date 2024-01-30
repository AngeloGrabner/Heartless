#include "TextBox.h"


ui::TextBox::TextBox(Widget* parent, SDL_FRect area, const std::string& name, int textureID, WidgetPostion x, WidgetPostion y, const std::string& text, Color textColor, float TextScale)
	: Frame(parent,area,name, textureID,x,y), mText(text), mTextColor(textColor), mTextScale(TextScale)
{
}

void ui::TextBox::Draw()
{
	if (!mActive)
		return;
	bool flag = mChildrinDrawFlag;
	mChildrinDrawFlag = true;
	Frame::Draw();
	auto temp = GetInnerRect();
	SDL_Rect textBox = { (int)temp.x, (int)temp.y,(int)temp.w,(int)temp.h };
	FontWriter::DrawText(textBox, mText, { mTextScale,mTextScale }, mTextColor);
	if (!flag)
		mChildrinDrawFlag = false;
	DrawChildrin();
}

void ui::TextBox::DebugDraw()
{
	bool flag = mChildrinDebugDrawFlag;
	mChildrinDebugDrawFlag = true;
	Frame::DebugDraw();
	if (!flag)
		mChildrinDebugDrawFlag = false;
	DebugDrawChildrin();
}

ui::TextBox* ui::TextBox::SetBackground(int texuteId, float scale)
{
	mTexId = texuteId;
	mScale = scale;
	return this;
}

ui::TextBox* ui::TextBox::SetText(const std::string& text, Color c, float scale)
{
	mText = text;
	mTextColor = c;
	mTextScale = scale;
	return this;
}
