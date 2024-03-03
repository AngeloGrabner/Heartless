#pragma once
#include <SDL.h>
#include <limits>
#include "../sdl_type_serializer.h"

class Camera
{
	SDL_FPoint mPos = { 0,0 };
	SDL_FPoint mScale = { 1.0f,1.0f };
	SDL_FPoint mWorldSize = { -1,-1 };
	SDL_Point mScreenSize = { -1, -1 };
	bool mClap = true;
	bool mScaleToMid = true; // not implemented, see //hier in .cpp 
public:
	Camera() = default;
	Camera(SDL_FPoint worldSize, SDL_FPoint pos = { 0.0f,0.0f },
		SDL_FPoint scale = { 1.0f,1.0f });

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);

	void Handle(const SDL_Event* e);
	void Update();
	void Clamp(bool Do = true);

	const SDL_FPoint& GetPos() const;
	const SDL_FPoint& GetScale() const;
	SDL_FRect GetRect();

	void SetPos(SDL_FPoint pos);
	//pos += dPos
	void SetPosRel(SDL_FPoint dPos);
	void SetScale(SDL_FPoint scale);
	//scale *= dScale
	void SetScaleRel(SDL_FPoint dScale);
	void SetTarget(SDL_FRect target);
	//not in tile space
	void SetWorldSize(SDL_FPoint sizeInWorldSpace);

private:
	void clamp();
	
};

template<class Archive>
inline void Camera::save(Archive& ar) const
{
	ar(mPos, mScale, mClap, mScaleToMid, mWorldSize);
}

template<class Archive>
inline void Camera::load(Archive& ar)
{
	ar(mPos, mScale, mClap, mScaleToMid, mWorldSize);
}
