#include "Player.h"
#include "../Scene.h"
#include "../../Event.h"

Player::Player(SDL_FRect hitbox, int textureId, SDL_FRect drawBoxOffset)
    : Creature(hitbox, textureId, drawBoxOffset)
{
}

Player::Player(SDL_FRect hitbox, int textureID, Milliseconds frameDuation, unsigned int frameCount, unsigned int animationCount)
    : Creature(hitbox, textureID, frameDuation, frameCount, animationCount)
{
}

bool Player::Update()
{
    if (!Creature::Update())
    {
        EventBuilder::PlayerDied(mId);
        
    }
    Scene::GetMut()->GetCamera().SetTarget(mHitBox);
    return true;
}
