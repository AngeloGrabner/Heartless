#include "Creature.h"
#include "../../Timer.h"

Creature::Creature(SDL_FRect hitbox, int textureId, SDL_FRect drawBoxOffset)
	: Entity(hitbox,textureId,drawBoxOffset)
{
}

Creature::Creature(SDL_FRect hitbox, int textureID, Milliseconds frameDuation, unsigned int frameCount, unsigned int animationCount)
	: Entity(hitbox,textureID,frameDuation,frameCount,animationCount)
{
}

Creature* Creature::AddActionController(std::shared_ptr<ActionController> con)
{
	mController = con;
	return this;
}

bool Creature::Update()
{
	if (mAni)
		mAni->Update();
	if (mController)
	{
		mController->Update();
		auto dirCache = mDir;
		mDir = mController->GetDir();

		if (mAni)
		{
			if (mDir.x != dirCache.x || mDir.y != dirCache.y)
			{	
				if (mDir.y == 1)
				{
					//hard coded animation values see player.bmp or simular
					mAni->SetAnimation(0);
				}
				else if (mDir.y == -1)
				{
					mAni->SetAnimation(1);
				}
				else if (mDir.x == 1)
				{
					mAni->SetAnimation(2);
				}
				else if (mDir.x == -1)
				{
					mAni->SetAnimation(3);
				}

			}
			else if (mDir.x == 0 && mDir.y == 0)
			{
				mAni->ResetFrame();
			}
		}
	}
	auto dt = MStoF(Timer::GetGlobalDT());
	//todo: item useage

	//hier

	//temp
	mHitBox.x += mDir.x * mStats.spd * dt;
	mHitBox.y += mDir.y * mStats.spd * dt;
	mHitBox = Collide(mHitBox);

	return mStats.hp > 0;
}
