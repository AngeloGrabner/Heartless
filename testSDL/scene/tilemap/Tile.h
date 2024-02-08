#pragma once
#include <SDL.h>
#include <memory>
#include "cereal/types/polymorphic.hpp"
#include "../../sdl_type_serializer.h"
#include "../../Animation.h"

class Tile
{
public: 
	enum TopStatus
	{
		NO_TOP,
		TOP,
		TOP_ONTOP
	};
protected:
	int mTexId = -1;
	TopStatus mTop = NO_TOP;
	int mTexIdTop = -1;
	Animation mBottomAni, mTopAni;

	bool mIsSolid = false;
	uint8_t mLight = 0; //todo: add a tint for lighting in Draw()

public:
	Tile() = default;//for cereal
	Tile(int bottomTextureId, TopStatus ts = NO_TOP, int topTextureId = -1,
	bool solid = false, uint8_t light = 0);
	//use addanimation to set animations
	Tile* AddAnimation(Millis frameTime, unsigned int frameCount,
		unsigned int animationCount = 1, bool isBottom = true);

	virtual void Update();
	virtual void Draw(SDL_FRect area);
	virtual void DrawOnTop(SDL_FRect area);
	virtual void Handle(const SDL_Event* e);

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);

	void SetSolid(bool solid);
	void SetTopTexture(int textureId);
	void SetBottomTexture(int textureId);
	int GetTopTexture() const;
	int GetBottomTexture() const;

	bool IsSolid() const;
};

template<class Archive>
inline void Tile::save(Archive& ar) const
{
	ar(cereal::make_nvp("texId",mTexId),
		cereal::make_nvp("top",mTop),
		cereal::make_nvp("textIdTop",mTexIdTop),
		cereal::make_nvp("bottomAni",mBottomAni),
		cereal::make_nvp("topAni",mTopAni),
		cereal::make_nvp("isSolid",mIsSolid),
		cereal::make_nvp("light",mLight));
}

template<class Archive>
inline void Tile::load(Archive& ar)
{
	ar(mTexId, mTop, mTexIdTop, mBottomAni, mTopAni, mIsSolid, mLight);
}
