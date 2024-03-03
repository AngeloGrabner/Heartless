#include "Tilemap.h"
#include "../../Renderer.h"
Tilemap::Tilemap(unsigned short width, unsigned short height,
    SDL_FPoint tileSize, uint8_t skylight)
    : mSize({ int(width),int(height) }), mDrawSize(tileSize),
    mSkylight(std::min(skylight, MAX_LIGHT_LEVEL))
{
    mMap = std::vector<std::shared_ptr<Tile>>(mSize.x * mSize.y);
}

Tilemap::Tilemap(Tilemap&& other) noexcept
{
    if (this != &other)
    {
        swap(*this, other);
    }
}

Tilemap& Tilemap::operator=(Tilemap&& other) noexcept
{
    if (this != &other)
    {
        swap(*this, other);
    }
    return *this;
}

void Tilemap::Handle(const SDL_Event* e, SDL_FRect area)
{
    SDL_Rect idx = ViewToIndex(area);
    for (int y = std::max(0,idx.y); y < std::min(idx.y + idx.h,mSize.y); y++)
    {
        for (int x = std::max(0,idx.x); x < std::min(idx.x + idx.w,mSize.x); x++)
        {
            mMap[c21d(x, y)]->Handle(e);
        }
    }
}

void Tilemap::Update(SDL_FRect area)
{
    SDL_Rect idx = ViewToIndex(area);
    for (int y = std::max(0, idx.y); y < std::min(idx.y + idx.h, mSize.y); y++)
    {
        for (int x = std::max(0, idx.x); x < std::min(idx.x + idx.w, mSize.x); x++)
        {
            mMap[c21d(x, y)]->Update({ mDrawSize.x * x,mDrawSize.y * y, mDrawSize.x,mDrawSize.y });
        }
    }
}

void Tilemap::Draw(SDL_FRect view)
{
    SDL_Rect idx = ViewToIndex(view);

    for (int y = std::max(0, idx.y); y < std::min(idx.y + idx.h, mSize.y); y++)
    {
        for (int x = std::max(0, idx.x); x < std::min(idx.x + idx.w, mSize.x); x++)
        {
            mMap[c21d(x, y)]->Draw({mDrawSize.x*x,mDrawSize.y*y, mDrawSize.x,mDrawSize.y});
        }
    }
}

void Tilemap::DrawOnTop(SDL_FRect view)
{
    SDL_Rect idx = ViewToIndex(view);
    for (int y = std::max(0, idx.y); y < std::min(idx.y + idx.h, mSize.y); y++)
    {
        for (int x = std::max(0, idx.x); x < std::min(idx.x + idx.w, mSize.x); x++)
        {
            mMap[c21d(x, y)]->DrawOnTop({ mDrawSize.x * x,mDrawSize.y * y, mDrawSize.x,mDrawSize.y });
        }
    }
}

void Tilemap::DebugDraw(SDL_FRect view)
{
    SDL_Rect idx = ViewToIndex(view);
    Renderer::SetColor(BLUE);
    for (int y = std::max(0, idx.y); y < std::min(idx.y + idx.h, mSize.y); y++)
    {
        for (int x = std::max(0, idx.x); x < std::min(idx.x + idx.w, mSize.x); x++)
        {
            if (mMap[c21d(x, y)]->IsSolid())
                Renderer::FillRect(SDL_FRect{ mDrawSize.x * x,mDrawSize.y * y, mDrawSize.x,mDrawSize.y });
            else
                Renderer::DrawRect(SDL_FRect{mDrawSize.x * x,mDrawSize.y * y, mDrawSize.x,mDrawSize.y });
        }
    }
}

SDL_Point Tilemap::GetSize() const
{
    return mSize;
}

void Tilemap::Set(size_t x, size_t y, std::shared_ptr<Tile> heapPtr)
{
    if (x >= mSize.x || y >= mSize.y)
        throw;
    mMap[c21d(x, y)] = heapPtr;
}

std::shared_ptr<Tile> Tilemap::Get(size_t x, size_t y, bool worldSpcae)
{
    if (worldSpcae)
    {
        x /= mDrawSize.x;
        y /= mDrawSize.y;
    }


    SDL_assert(!(x >= mSize.x || y >= mSize.y));

    return mMap[c21d(x, y)];

}

void Tilemap::Get(SDL_FRect area, SDL_Point& arrSize, std::vector<std::shared_ptr<Tile>>& data)
{
    SDL_Rect idx = ViewToIndex(area);
    arrSize.x = std::min(idx.w,mSize.x);
    arrSize.y = std::min(idx.h,mSize.y);
    
    for (int y = std::max(0, idx.y); y < std::min(idx.y + idx.h, mSize.y); y++)
    {
        for (int x = std::max(0, idx.x); x < std::min(idx.x + idx.w, mSize.x); x++)
        {
            data.push_back(mMap[c21d(x, y)]);

        }
    }
}

void Tilemap::Get(SDL_FRect area, SDL_Point& arrSize,std::vector< std::shared_ptr<Tile>>& data) const
{
    SDL_Rect idx = ViewToIndex(area);
    arrSize.x = std::min(idx.w, mSize.x);
    arrSize.y = std::min(idx.h, mSize.y);

    for (int y = std::max(0, idx.y); y < std::min(idx.y + idx.h, mSize.y); y++)
    {
        for (int x = std::max(0, idx.x); x < std::min(idx.x + idx.w, mSize.x); x++)
        {
            data.push_back(mMap[c21d(x, y)]);

        }
    }
}

void Tilemap::GetSolidAreas(SDL_FRect searchArea, std::vector<SDL_FRect>& areas) const
{
    SDL_Rect idx = ViewToIndex(searchArea);

    for (int y = std::max(0, idx.y); y < std::min(idx.y + idx.h, mSize.y); y++)
    {
        for (int x = std::max(0, idx.x); x < std::min(idx.x + idx.w, mSize.x); x++)
        {
            if (mMap[c21d(x, y)]->IsSolid())
                areas.push_back(SDL_FRect(x * mDrawSize.x, y * mDrawSize.y, mDrawSize.x, mDrawSize.y));
        }
    }
}

inline size_t Tilemap::c21d(size_t x, size_t y) const
{
    return x + mSize.x * y;
}

inline SDL_Rect Tilemap::ViewToIndex(SDL_FRect area) const
{
    SDL_Rect a;
    a.x = (int)(area.x / mDrawSize.x);
    a.y = (int)(area.y / mDrawSize.y);
    a.w = (int)(area.w / mDrawSize.x + area.x / mDrawSize.x - a.x) + 1;
    a.h = (int)(area.h / mDrawSize.y + area.y / mDrawSize.y - a.y) + 1;
    return a;
}

void swap(Tilemap& one, Tilemap& other)
{
    std::swap(one.mDrawSize, other.mDrawSize);
    std::swap(one.mSkylight, other.mSkylight);
    std::swap(one.mSize, other.mSize);
    std::swap(one.mMap, other.mMap);
}
