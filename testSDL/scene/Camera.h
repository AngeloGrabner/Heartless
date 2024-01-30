#pragma once
#include <SDL.h>
#include <limits>
#include "../sdl_type_serializer.h"

class Camera
{
	SDL_FPoint mPos = { 0,0 };
	SDL_FPoint mScale = { 1.0f,1.0f };
	SDL_FPoint mWorldSize = { 0,0 };
	SDL_FRect mTarget = { FLT_MIN,FLT_MIN,0,0 };
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
	void SetPos(SDL_FPoint pos);
	void SetScale(SDL_FPoint scale);
	const SDL_FRect GetRect() const;
};

template<class Archive>
inline void Camera::save(Archive& ar) const
{
	ar(mPos, mScale, mClap, mScaleToMid);
}

template<class Archive>
inline void Camera::load(Archive& ar)
{
	ar(mPos, mScale, mClap, mScaleToMid);
}
