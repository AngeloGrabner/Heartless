#include "InputField.h"
#include "../Input.h"
#include "../TextureManager.h"
#include "../Event.h"
#include "Font.h"

ui::InputField::InputField(Widget* parent, SDL_FRect area, const std::string& name, WidgetPostion x, WidgetPostion y, int textureID, int fontID, const std::string& defaultText)
	: Widget(parent,area,name,x,y), mTexId(textureID), mFontId(fontID), mText(defaultText)
{
}

ui::InputField* ui::InputField::SetTexture(int textureId)
{
	mTexId = textureId;
	return this;
}

ui::InputField* ui::InputField::SetText(const std::string& defaultText)
{
	mText = defaultText;
	mCursor += defaultText.size();
	return this;
}

ui::InputField* ui::InputField::SetFont(int fontId)
{
	mFontId = fontId;
	return this;
}

void ui::InputField::Update()
{
	if (!mActive)
		return;
	Widget::Update();
	if (contain(mArea, Input::GetMousePos()) &&
		Input::GetMouse(Input::LMB, LAYER_UI).Down())
	{
		mSelected = true;
		Input::SetKeyboardMode(true);
		return;
	}
	if (mSelected)
	{
		if (Input::GetMouse(Input::LMB, LAYER_UI).Down())
		{
			Input::Handled(Input::LMB, LAYER_UI);
			Input::SetKeyboardMode(false);
			mSelected = false;
			return;
		}
		auto str = Input::GetTextChunk();
		auto nlPos = str.find_first_of('\n');
		if (nlPos != std::string::npos )
		{
			mSelected = false;
			str = str.substr(0, nlPos);

			EventBuilder::InputField(mName,mText);

			Input::SetKeyboardMode(false);
		}
		mText.insert((size_t)mCursor,str);
		mCursor += str.size();
		auto retPos = mText.find_first_of(8);
		while (std::string::npos != retPos)
		{
			mText.erase(retPos);
			if (mText.size() != 0)
			{
				mText.erase(--retPos);
				mCursor--;
			}
			mCursor--;
			retPos = mText.find_first_of(8);
		}
	}
}

void ui::InputField::Draw()
{
	auto tex = TextureManager::Get(mTexId);
	tex.rect.w /= 3;
	SDL_FRect area = mArea;
	float yScale = area.h/tex.rect.h;
	{ // draw the box 
		area.w = tex.rect.w * yScale;
		Renderer::DrawTexture(tex, area);

		tex.rect.x += tex.rect.w;
		area.x += area.w;
		area.w = mArea.w - area.w * 2.0f;
		Renderer::DrawTexture(tex, area);

		tex.rect.x += tex.rect.w;
		area.w = tex.rect.w * yScale;
		area.x = mArea.x + mArea.w - area.w;
		Renderer::DrawTexture(tex, area);
	}
	
	const float offset = 2;
	SDL_Rect textArea = cast<SDL_FRect,SDL_Rect>(GetInnerArea());
	auto fonstCache = FontWriter::GetFont();
	if (mFontId != -1)
	{
		FontWriter::SetFont(mFontId);
	}
	float textScale = textArea.h / (float)FontWriter::GetSize().y;

	int startTextAt = 0;
	int textLength = 0;
	for (int i = 0; i < mText.size(); i++)
	{
		textLength+= (int)((FontWriter::GetSize(mText[i]).x +
			FontWriter::GetCharSpacing().x) * textScale);
	}
	while (textLength > textArea.w)
	{
		textLength -= (int)((FontWriter::GetSize(mText[startTextAt]).x + FontWriter::GetCharSpacing().x) * textScale);
		startTextAt++;
	}

	FontWriter::DrawText(textArea, mText.substr(startTextAt), {textScale,textScale}, mTextTint, true);
	if (mSelected)
	{
		int cursorX = textArea.x;
		for (int i = startTextAt; i < mCursor; i++)
			cursorX += (int)((FontWriter::GetSize(mText[i]).x+FontWriter::GetCharSpacing().x) * textScale);
		Renderer::SetColor(mTextTint);
		Renderer::DrawLine(SDL_Point(cursorX, textArea.y), SDL_Point(cursorX, textArea.y + textArea.h));
	}
	FontWriter::SetFont(fonstCache);
	DrawChildrin();
}
