#include "Camera.h"
#include "../Utility.h"
#include "../Window.h"
#include "../Renderer.h"
#include <algorithm>

Camera::Camera(SDL_FPoint worldSize, SDL_FPoint pos, SDL_FPoint scale)
	: mPos(pos),mScale(scale), mWorldSize(worldSize)
{

}

void Camera::Handle(const SDL_Event* e)
{
	if (e->type == SDL_WINDOWEVENT)
	{
		if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e->window.event == SDL_WINDOWEVENT_RESIZED)
		{
			mScreenSize.x = e->window.data1;
			mScreenSize.y = e->window.data2;
		}
	}
}

void Camera::Update()
{
	Renderer::SetOffset(mPos);
	Renderer::SetZoom(mScale);
	//std::cout << "pos:" << mPos.x << " " << mPos.y << " scale: " << mScale.x << " " << mScale.y << "\n";
}

void Camera::Clamp(bool Do)
{
	mClap = Do;
}

const SDL_FPoint& Camera::GetPos() const
{
	return mPos;
}
const SDL_FPoint& Camera::GetScale() const
{
	return mScale;
}

void Camera::SetPos(SDL_FPoint pos)
{
	mPos = pos;
	clamp();
}
void Camera::SetPosRel(SDL_FPoint dPos)
{
	mPos.x += dPos.x;
	mPos.y += dPos.y;
	clamp();
}
void Camera::SetScale(SDL_FPoint scale)
{
	mScale = scale;
	clamp();
}

void Camera::SetScaleRel(SDL_FPoint dScale)
{
	mScale.x *= dScale.x;
	mScale.y *= dScale.y;
	clamp();
}

void Camera::SetTarget(SDL_FRect target)
{
	_UNLIKELY
	if (mScreenSize.x == -1 || mScreenSize.y == -1)
		mScreenSize = Window::GetSize();

	mPos.x = target.x - (mScreenSize.x / mScale.x)/ 2 + target.w / 2.0f;
	mPos.y = target.y - (mScreenSize.y / mScale.y) / 2 + target.h / 2.0f;
	
	clamp();
}

void Camera::SetWorldSize(SDL_FPoint sizeInWorldSpace)
{
	mWorldSize = sizeInWorldSpace;
}

void Camera::clamp()
{
	if (mClap)
	{
		mPos.x = std::clamp(mPos.x, 0.0f, mWorldSize.x - mScreenSize.x / mScale.x);
		mPos.y = std::clamp(mPos.y, 0.0f, mWorldSize.y - mScreenSize.y / mScale.y);
	}
}

SDL_FRect Camera::GetRect()
{
	_UNLIKELY
	if (mScreenSize.x == -1 || mScreenSize.y == -1)
		mScreenSize = Window::GetSize();
	return SDL_FRect(
		mPos.x,
		mPos.y,
		mScreenSize.x / mScale.x,
		mScreenSize.y / mScale.y);
}
