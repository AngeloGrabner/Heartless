#pragma once
#include "Tile.h"

class Entity;

class SceneSwapTile : public Tile
{
	std::string mFileName;
	SDL_FPoint mDest = { 0,0 };
	std::list<std::shared_ptr<Entity>> mTransferList;
public:
	SceneSwapTile() = default;
	SceneSwapTile(int bottomTextureId, TopStatus ts = NO_TOP, int topTextureId = -1,
		bool solid = false, uint8_t light = 0);

	SceneSwapTile* SetDestName(const std::string& fileNameWithoutFileExtension);
	SceneSwapTile* SetDestPos(SDL_FPoint destPos);

	void Update(SDL_FRect area) override;

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);

private:
	bool Comp(const Tile& other) override;
};

template<class Archive>
inline void SceneSwapTile::save(Archive& ar) const
{
	ar(cereal::base_class(this), mFileName, mDest);
}

template<class Archive>
inline void SceneSwapTile::load(Archive& ar)
{
	ar(cereal::base_class(this), mFileName, mDest);
}
