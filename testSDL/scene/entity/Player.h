#pragma once
#include "Creature.h"

class Player : public Creature 
{
public:
	ENTITY_TYPE;
	//just for cereal to construct
	Player() = default;
	Player(SDL_FRect hitbox, int textureId = -1, SDL_FRect drawBoxOffset = { 0,0,0,0 });
	Player(SDL_FRect hitbox, int textureID, Milliseconds frameDuation,
		unsigned int frameCount, unsigned int animationCount = 1);
	bool Update() override;
	//void Draw() override;

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);
};

CEREAL_REGISTER_TYPE(Player);

template<class Archive>
inline void Player::save(Archive& ar) const
{
	ar(cereal::base_class<Creature>(this));
}

template<class Archive>
inline void Player::load(Archive& ar)
{
	ar(cereal::base_class<Creature>(this));
}
