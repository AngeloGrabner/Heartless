#pragma once
#include <vector>
#include "sdl_type_serializer.h"
#include "TextureManager.h"
#include "Timer.h"

// animations need to be tidly packed
// frames from left to right
// animations from top to bottom 
// Animation
// {
//	{frame0,frame1,frameN},
//	{frame0, frame1, frameN},
//	{frame0, frame1, frameN},
// }

class Animation
{
	bool mInited = false;
	int mTexId = -1;
	SDL_Rect mFrameArea = { 0,0,0,0 };
	SDL_Point mIndex = { 0,0 };
	SDL_Point mSteps = { 0,0 };
	Duration mDur;
	Texture mRetMe;
public: 
	Animation() = default;
	Animation(int textureID,Milliseconds frameDuation,
		unsigned int frameCount, unsigned int animationCount = 1);

	bool IsInited() const;
	Texture Get() const;
	int GetTextureId() const;
	Millis GetFrameTime() const;
	void NextFrame();
	void NextAnimation();
	void Update ();
	void ResetFrame();
	void ResetAnimation();
	void SetAnimation(size_t animationId);
	void SetFrame(size_t frame);
	void SetFrameTime(Millis frametime);


	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);
};

template<class Archive>
inline void Animation::save(Archive& ar) const
{
	ar(mInited);
	if (mInited)
	{
		ar(mTexId, mFrameArea, mSteps, mDur);
	}
}

template<class Archive>
inline void Animation::load(Archive& ar)
{
	ar(mInited);
	if (mInited)
	{
		ar(mTexId, mFrameArea, mSteps, mDur);
	}
}
