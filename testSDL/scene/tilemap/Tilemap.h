#pragma once
#include "Tile.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#define MAX_LIGHT_LEVEL (uint8_t)(16)


class Tilemap 
{
	//constexpr SDL_Point 
	SDL_FPoint mDrawSize = { 0,0 };
	uint8_t mSkylight = 0;
	SDL_Point mSize = { 0,0 };
	std::vector<std::shared_ptr<Tile>> mMap;
public:
	Tilemap() = default;
	Tilemap(unsigned short width, unsigned short height, SDL_FPoint tileSize, uint8_t skylight = MAX_LIGHT_LEVEL);
	Tilemap(const Tilemap&) = delete;
	Tilemap(Tilemap&& other) noexcept;
	Tilemap& operator=(const Tilemap&) = delete;
	Tilemap& operator=(Tilemap&& other) noexcept;

	friend void swap(Tilemap& one, Tilemap& other);

	void Handle(const SDL_Event* e, SDL_FRect area);
	void Update(SDL_FRect area);
	void Draw(SDL_FRect view);
	void DrawOnTop(SDL_FRect view);
	void DebugDraw(SDL_FRect view);

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);

	SDL_Point GetSize() const;
	void Set(size_t x, size_t y, std::shared_ptr<Tile> heapPtr);
	std::shared_ptr<Tile> Get(size_t x, size_t y, bool worldSpace = true);
	void Get(SDL_FRect area, SDL_Point& arrSize, std::vector<std::shared_ptr<Tile>>& data);
	//vector<const T> is not allowed thats why it has to be mutable
	void Get(SDL_FRect area, SDL_Point& arrSize, std::vector<std::shared_ptr<Tile>>& data) const;
	void GetSolidAreas(SDL_FRect searchArea, std::vector<SDL_FRect>& areas) const;
private:
	//convertTo1d
	inline size_t c21d(size_t x, size_t y) const;
	inline SDL_Rect ViewToIndex(SDL_FRect area) const;
};

template<class Archive>
inline void Tilemap::save(Archive& ar) const
{
	ar(cereal::make_nvp("drawSize", mDrawSize),
		cereal::make_nvp("skylight",mSkylight),
		cereal::make_nvp("size", mSize),
		cereal::make_nvp("map",mMap));
}

template<class Archive>
inline void Tilemap::load(Archive& ar)
{
	ar(mDrawSize, mSkylight, mSize);

	mMap = std::vector<std::shared_ptr<Tile>>(mSize.x * mSize.y);
	ar(mMap);
}
