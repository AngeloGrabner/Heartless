#include "Camera.h"
#include "../Event.h"
#include "../Utility.h"
#include "../Window.h"
#include "../Renderer.h"

Camera::Camera(SDL_FPoint worldSize, SDL_FPoint pos, SDL_FPoint scale)
	: mPos(pos),mScale(scale), mWorldSize(worldSize)
{

}

void Camera::Handle(const SDL_Event* e)
{
	EventReceiver::CameraTargetRect(e, mTarget);
	if (EventReceiver::CameraAbsPos(e, mPos))
	{
		mTarget.x = FLT_MIN;
		mTarget.y = FLT_MIN;
		return;
	}
	SDL_FPoint p = { 0,0 };
	if (EventReceiver::CameraRelPos(e, p))
	{
		mTarget.x = FLT_MIN;
		mTarget.y = FLT_MIN;
		mPos.x += p.x;
		mPos.y += p.y;
		return;
	}
	auto tempS = mScale;
	bool csrel = EventReceiver::CameraScaleRel(e, p);
	if (csrel)
	{
		mScale.x *= p.x;
		mScale.y *= p.y;
	}
	if (EventReceiver::CameraScaleAbs(e, mScale) || csrel)
	{
		auto winSize = Window::GetSize();
		if (mScaleToMid)
		{
			SDL_FPoint preCamSize = { winSize.x / tempS.x, winSize.y / tempS.y };
			SDL_FPoint postCamSize = { winSize.x / mScale.x, winSize.y / mScale.y };

			mPos.x += (preCamSize.x - postCamSize.x) / 2.0f;
			mPos.y += (preCamSize.y - postCamSize.y) / 2.0f;
		}
		else
		{
			//hier 
			//scale to mouse
		}
	}
}

void Camera::Update()
{
	if (mTarget.x != FLT_MIN && mTarget.y != FLT_MIN)
	{
		auto winSize = Window::GetSize();
		//calc camera size
		winSize.x /= mScale.x;
		winSize.y /= mScale.y;
		mPos.x = mTarget.x - winSize.x / 2 + mTarget.w / 2;
		mPos.y = mTarget.y - winSize.y / 2 + mTarget.h / 2;

	}
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
}
void Camera::SetScale(SDL_FPoint scale)
{
	mScale = scale;
}

const SDL_FRect Camera::GetRect() const
{
	auto winSize = Window::GetSize();
	//calc camera size
	winSize.x /= mScale.x;
	winSize.y /= mScale.y;
	return SDL_FRect(mPos.x,mPos.y,winSize.x,winSize.y);
}
