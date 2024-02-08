#include "Ui.h"
#include "../Window.h"

ui::Widget::Widget(ui::Widget* parent, SDL_FRect area,
	const std::string& name,
	WidgetPostion x, WidgetPostion y)
	: mArea(area), mName(name)
{
	SDL_FRect pArea, pBorder;
	if (parent)
	{
		pBorder = parent->mBorder;
		pArea = parent->mArea;
		parent->mChilds.push_back(std::move(std::unique_ptr<ui::Widget>(this)));
		if (parent->GetDepth() +1 < MAX_UI_DEPTH)
		{
			mDepth = parent->GetDepth() +1;
		}
	#ifdef _DEBUG
		else
		{
			throw; 
		}
	#endif
	}
	else
	{
		mDepth = 0;
		pArea = SDL_FRect(0, 0, Window::GetSize().x, Window::GetSize().y);
		pBorder = SDL_FRect(5, 5, 5, 5);
	}
	switch (x)
	{
	case ui::WIDGET_CENTER:
		mArea.x += pBorder.x + pArea.x + ( pArea.w-pBorder.w*2) / 2 - mArea.w / 2;
		break;
	case ui::WIDGET_LEFT:
		mArea.x += pBorder.x + pArea.x;
		break;
	case ui::WIDGET_RIGHT:
		mArea.x += pBorder.x + pArea.x + (pArea.w - pBorder.w*2) - mArea.w;
		break;
	default:
		throw;
	}
	switch (y)
	{
	case ui::WIDGET_CENTER:
		mArea.y += pBorder.y + pArea.y + (pArea.h - pBorder.h*2) / 2 - mArea.h / 2;
		break;
	case ui::WIDGET_TOP:
		mArea.y += pBorder.y + pArea.y;
		break;
	case ui::WIDGET_BOTTOM:
		mArea.y += pBorder.y + pArea.y + (pArea.h - pBorder.h*2)- mArea.h;
		break;
	default:
		throw;
	}
}

ui::Widget::Widget(Widget&& other) noexcept
	: mArea(other.mArea), 
	mActive(other.mActive), 
	mChilds(std::move(other.mChilds)),
	mName(std::move(other.mName)),
	mBorder(other.mBorder)
{}

ui::Widget& ui::Widget::operator=(Widget&& other) noexcept
{
	if (this == &other)
	{
		mArea = other.mArea;
		mActive = other.mActive;
		mChilds = std::move(other.mChilds);
		mName = std::move(other.mName);
		mBorder = other.mBorder;
	}
	return *this;
}

void ui::Widget::Move(int xOffset, int yOffset)
{
	mArea.x += xOffset;
	mArea.y += yOffset;
	for (int i = 0; i < mChilds.size(); i++)
	{
		if (mChilds[i])
		{
			mChilds[i]->Move(xOffset, yOffset);
		}
	}
}


const std::string& ui::Widget::GetName() const
{
	return mName;
}

const SDL_FRect& ui::Widget::GetArea() const
{
	return mArea;
}

SDL_FRect ui::Widget::GetInnerArea() const
{
	return SDL_FRect(mArea.x + mBorder.x, mArea.y + mBorder.y, mArea.w - mBorder.w*2, mArea.h - mBorder.h*2);
}

int ui::Widget::GetDepth() const
{
	return mDepth;
}

ui::Widget* ui::Widget::SetBorder(SDL_FRect size)
{
	mBorder = size;
	return this;
}

ui::Widget* ui::Widget::Activate(bool active)
{
	mActive = active;
	return this;
}

void ui::Widget::Activate(const std::string& name, bool active)
{
	if (mName == name)
	{
		mActive = active;
	}
	for (int i = 0; i < mChilds.size(); i++)
	{
		if (mChilds[i])
			mChilds[i]->Activate(name, active);
	}
}


void ui::Widget::Update()
{
	mChildrinDrawFlag = false;
	mChildrinDrawFlag = false;
	UpdateChildrin();
}

void ui::Widget::DebugDraw()
{
	Renderer::SetColor(RED);
	Renderer::DrawRect(mArea);
	Renderer::SetColor(GREEN);
	Renderer::DrawRect(GetInnerArea());
	DebugDrawChildrin();
}

void ui::Widget::DrawChildrin() 
{
	if (!mChildrinDrawFlag)
	{
		for (int i = 0; i < mChilds.size(); i++)
		{
			mChilds[i]->Draw();
		}
		mChildrinDrawFlag = true;
	}
}

void ui::Widget::UpdateChildrin()
{
	for (int i = 0; i < mChilds.size(); i++)
	{
		mChilds[i]->Update();
	}
}

void ui::Widget::DebugDrawChildrin()
{
	if (!mChildrinDebugDrawFlag)
	{
		for (int i = 0; i < mChilds.size(); i++)
		{
			mChilds[i]->DebugDraw();
		}
	}
}

