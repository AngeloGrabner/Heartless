#include "SimpleEnemyController.h"
#include "../../Scene.h"
#include "../Player.h"

void SimpleEnemyController::Update(Entity* owner)
{
	auto playerPos = mid<SDL_FPoint, SDL_FRect>(Scene::Get()->GetPlayer()->GetHitBox());

	auto ownPos = mid<SDL_FPoint, SDL_FRect>(owner->GetHitBox());

	mDir.x = playerPos.x - ownPos.x;
	mDir.y = playerPos.y - ownPos.y;
	if (mDir.x != 0 && mDir.y != 0)
		mDir = norm(mDir);
}
