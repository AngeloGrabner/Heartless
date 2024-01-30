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
		mDir = mController->GetDir();
	}
	auto dt = MStoF(Timer::GetGlobalDT());
	//todo: item useage
	

	//temp
	mHitBox.x += mDir.x * mStats.spd * dt;
	mHitBox.y += mDir.y * mStats.spd * dt;
	mHitBox = Collide(mHitBox);

	return mStats.hp > 0;
}
