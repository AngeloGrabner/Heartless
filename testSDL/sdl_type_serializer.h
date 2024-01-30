#pragma once
#include <SDL_rect.h>
#include <cereal/cereal.hpp>

template<class Archive>
void serialize(Archive& ar, SDL_Point& p)
{
	ar(cereal::make_nvp("x",p.x), cereal::make_nvp("y",p.y));
}
template<class Archive>
void serialize(Archive& ar, SDL_FPoint& p)
{
	ar(cereal::make_nvp("x", p.x), cereal::make_nvp("y", p.y));
}
template<class Archive>
void serialize(Archive& ar, SDL_Rect& r)
{
	ar(cereal::make_nvp("x",r.x),
		cereal::make_nvp("y",r.y),
		cereal::make_nvp("w",r.w),
		cereal::make_nvp("h", r.h));
}
template<class Archive>
void serialize(Archive& ar, SDL_FRect& r)
{
	ar(cereal::make_nvp("x", r.x),
		cereal::make_nvp("y", r.y),
		cereal::make_nvp("w", r.w),
		cereal::make_nvp("h", r.h));
}