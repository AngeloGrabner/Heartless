#include "Select.h"
#include "../Input.h"
#include "../TextureManager.h"
#include "Font.h"

namespace ui
{
	Select::Select(Widget* parent,
		SDL_FRect area,
		const std::string& name,
		int textureId,
		WidgetPostion x,
		WidgetPostion y,
		const std::vector<std::string>& options,
		Color TextColor,
		Color selectionBackgroundColor,
		WidgetPostion alignment)

		: Widget(parent,area,name,x,y), mOptions(options),
		mTextColor(TextColor), mSelectionBackgroundColor(selectionBackgroundColor),
		mAlignment(alignment), mTexId(textureId)
	{
		mTextScale = GetInnerArea().h / (float)FontWriter::GetSize().y;
	}

	Select::Select(Select&& other) noexcept
		: Widget(std::move(other)),mOptions(std::move(other.mOptions)),
		mTextColor(other.mTextColor), mSelectionBackgroundColor(other.mSelectionBackgroundColor),
		mAlignment(other.mAlignment), mTexId(other.mTexId),mTextScale(other.mTextScale),
		mIndex(other.mIndex)
	{

	}

	Select& Select::operator=(Select&& other) noexcept
	{
		if (this != &other)
		{
			Widget::operator=(std::move(other));
			mOptions = std::move(other.mOptions);
			mTextColor = other.mTextColor;
			mSelectionBackgroundColor = other.mSelectionBackgroundColor;
			mAlignment = other.mAlignment;
			mTexId = other.mTexId;
			mTextScale = other.mTextScale;
			mIndex = other.mIndex;
		}
		return *this;
	}

	void Select::Update()
	{
		if (!mActive)
			return;
		Widget::Update();
		auto mousePos = Input::GetMousePos();
		//click to expend
		if (!mShowOptions && contain(mArea, mousePos) && Input::GetMouse(Input::LMB, LAYER_UI).Down())
		{
			Input::Handled(Input::LMB, LAYER_UI);
			mShowOptions = true;
			return;
		}
		auto area = mArea;
		area.h *= mOptions.size();
		if (mShowOptions && contain(area, mousePos) && Input::GetMouse(Input::LMB, (Input::Layer)(Input::UI-MAX_UI_DEPTH)).Down())
		{
			Input::Handled(Input::LMB, (Input::Layer)(Input::UI - MAX_UI_DEPTH));
			mIndex = (int)((mousePos.y - area.y) / mArea.h);
			EventBuilder::Select(mName, mOptions[mIndex]);
			mShowOptions = false;
			return;
		}
	}

	void Select::Draw()
	{
		if (!mActive)
			return;
		if (mTexId != -1)
		{
			auto tex = TextureManager::Get(mTexId);
			Renderer::DrawTexture(tex, mArea);
		}

		if (mShowOptions)
		{	
			auto mousePos = Input::GetMousePos();
			auto area = mArea;
			
			int idx = (mousePos.y-mArea.y) / mArea.h;

			auto innerArea = GetInnerArea();

			for (int i = 0; i < mOptions.size(); i++)
			{
				Renderer::SetColor(mSelectionBackgroundColor);
				if (i == idx)
				{
					Color c = {
						(uint8_t)std::min(mSelectionBackgroundColor.r + 20,255),
						(uint8_t)std::min(mSelectionBackgroundColor.g + 20,255),
						(uint8_t)std::min(mSelectionBackgroundColor.b + 20,255)
					};
					Renderer::SetColor(c);
				}
				Renderer::FillRect(area);
				FontWriter::DrawText(cast<SDL_FRect,SDL_Rect>(innerArea), mOptions[i], { mTextScale,mTextScale }, mTextColor, true);
				area.y += area.h;
				innerArea.y += area.h;
			}
		}
		else
		{
			FontWriter::DrawText(cast<SDL_FRect,SDL_Rect>(mArea), mOptions[mIndex], {mTextScale,mTextScale}, mTextColor, true);
		}
		
		DrawChildrin();
	}

	Select* Select::SetColor(Color textColor, Color selectionBackgroundColor)
	{
		mTextColor = textColor;
		mSelectionBackgroundColor = selectionBackgroundColor;
		return this;
	}

	Select* Select::SetOptions(const std::vector<std::string>& options)
	{
		mOptions = options;
		return this;
	}

	Select* Select::AddOption(const std::string& option)
	{
		mOptions.push_back(option);
		return this;
	}

	Select* Select::SetAlignment(WidgetPostion alignment)
	{
		mAlignment = alignment;
		return this;
	}
}