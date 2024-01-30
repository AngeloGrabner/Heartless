#include "Animation.h"

Animation::Animation(int textureID, Milliseconds frameDuation, unsigned int frameCount, unsigned int animationCount)
	: mInited(true), mTexId(textureID), mDur(frameDuation)
{
	mSteps = {(int)frameCount,(int)animationCount};
}

bool Animation::IsInited() const
{
	return mInited;
}

Texture Animation::Get() const
{
	return mRetMe;
}

int Animation::GetTextureId() const
{
	return mTexId;
}

void Animation::NextFrame()
{
	mIndex.x++;
	mIndex.x %= mSteps.x;
}

void Animation::NextAnimation()
{
	mIndex.y++;
	mIndex.y %= mSteps.y;
}

void Animation::Update()
{
	if (!mDur.Update())
	{
		NextFrame();
		//get texture every frame cause it might have been deallocated in between frames
		//migth remove deallocation of texture managers texutres
		mRetMe = TextureManager::Get(mTexId);

		mRetMe.rect.x = mRetMe.rect.x + mRetMe.rect.w / mSteps.x * mIndex.x;
		mRetMe.rect.y = mRetMe.rect.y + mRetMe.rect.h / mSteps.y * mIndex.y;
	}
}

void Animation::ResetFrame()
{
	mIndex.x = 0;
}

void Animation::ResetAnimation()
{
	mIndex.y = 0;
}

void Animation::SetAnimation(size_t animationId)
{
#if _DEBUG
	if (animationId > mSteps.y)
		throw;
#endif
	mIndex.y = animationId;
}

void Animation::SetFrame(size_t frame)
{
	mIndex.x = frame;
	mIndex.x %= mSteps.x;
}
