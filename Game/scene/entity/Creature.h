#pragma once
#include "Entity.h"
#include "controller/ActionController.h"

class Creature : public Entity
{
protected:
	std::shared_ptr<ActionController> mController;
	//inventory
public:
	//just for cereal to construct
	Creature() = default;
	Creature(SDL_FRect hitbox, int textureId = -1, SDL_FRect drawBoxOffset = { 0,0,0,0 });
	Creature(SDL_FRect hitbox, int textureID, Milliseconds frameDuation,
		unsigned int frameCount, unsigned int animationCount = 1);

	Creature* AddActionController(std::shared_ptr<ActionController> con);

	bool Update() override;

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);
};

CEREAL_REGISTER_TYPE(Creature);

template<class Archive>
inline void Creature::save(Archive& ar) const
{
	ar(cereal::base_class<Entity>(this), cereal::make_nvp("actionController",mController));
}

template<class Archive>
inline void Creature::load(Archive& ar)
{
	ar(cereal::base_class<Entity>(this), mController);
}
