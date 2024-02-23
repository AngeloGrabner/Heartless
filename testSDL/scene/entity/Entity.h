#pragma once
#include <SDL.h>
#include <bitset>
#include "../../Animation.h"
#include "../../sdl_type_serializer.h"
#include <memory>
#include "StatPack.h"

#include <cereal/types/bitset.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>


class Entity
{
public:
	enum Flags : uint8_t
	{
		ALIVE,
		HITTABLE,
		INTERACTABLE,
		CAN_PICKUP,

		LAST_INTERNAL //just for counting the members of Flags
	};
	static constexpr size_t InvalidId = 0;
protected:
	//relative to hitbox
	SDL_FRect mDrawBox = { 0,0,0,0 };
	SDL_FRect mHitBox = { 0,0,0,0 };
	StatPack mStats;
	SDL_FPoint mDir = { 0,0 };
	size_t mId = InvalidId;
	std::bitset<Flags::LAST_INTERNAL> mFlags;
	int mTexId = -1;
	std::unique_ptr<Animation> mAni;
private:
	static size_t sIdCounter; //todo reset when new scene is loaded
public:
	//just for cereal to construct
	Entity() = default;
	Entity(SDL_FRect hitbox, int textureId = -1, SDL_FRect drawBoxOffset = { 0,0,0,0 });
	Entity(SDL_FRect hitbox, int textureID, Milliseconds frameDuation,
		unsigned int frameCount, unsigned int animationCount = 1);
	Entity(const Entity&) = delete;
	Entity(Entity&& other) noexcept;
	Entity& operator=(const Entity*) = delete;
	Entity& operator=(Entity&& other) noexcept;
	//virtual ~Entity(); //not needed

	virtual void Handle(const SDL_Event* e);
	virtual bool Update() = 0;
	virtual void Draw();
	virtual void DebugDraw();

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);

	SDL_FRect AbsDrawBox() const;
	SDL_FRect GetHitBox() const;
	SDL_FRect& HitBox();

	Entity* SetStats(const StatPack& stats);
	StatPack GetStats() const;

	bool GetFlag(Flags f) const;
	Entity* SetFlag(Flags f, bool value);

	Entity* SetAnimation(std::unique_ptr<Animation> ani); // no getter so far needed
	
	Entity* SetDrawBox(SDL_FRect offsetbox);
	SDL_FRect GetDrawBox() const;
};

SDL_FRect Collide(SDL_FRect Pos);


template<class Archive>
inline void Entity::save(Archive& ar) const
{
	ar(cereal::make_nvp("drawbox",mDrawBox),
		cereal::make_nvp("hitbox",mHitBox),
		cereal::make_nvp("stats",mStats),
		cereal::make_nvp("dir",mDir),
		cereal::make_nvp("id",mId),
		cereal::make_nvp("flags", mFlags),
		cereal::make_nvp("texId",mTexId),
		cereal::make_nvp("ani",mAni));
}

template<class Archive>
inline void Entity::load(Archive& ar)
{
	ar(mDrawBox, mHitBox, mStats, mDir, mId, mFlags, mTexId, mAni);
}
