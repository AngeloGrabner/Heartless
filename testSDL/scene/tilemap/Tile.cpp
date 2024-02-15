#include "Tile.h"
#include "../../Renderer.h"
#include "Tilemap.h"

Tile::Tile(int bottomTextureId, TopStatus ts, int topTextureId,
    bool solid, uint8_t light)

    : mTexId(bottomTextureId), mTexIdTop(topTextureId), 
    mTop(ts), mLight(light), mIsSolid(solid)
{     
    SDL_assert(!(ts != NO_TOP && topTextureId < 0));
}

Tile* Tile::AddAnimation(Millis frameTime, unsigned int frameCount, unsigned int animationCount, bool isBottom)
{
    if (isBottom)
    {
        mBottomAni = Animation(mTexId, frameTime, frameCount, animationCount);
    }
    else
    {
        mTopAni = Animation(mTexId, frameTime, frameCount, animationCount);
    }
    return this;
}

void Tile::Update()
{
    if (mBottomAni.IsInited())
    {
        mBottomAni.Update();
    }
    if (mTopAni.IsInited())
    {
        mTopAni.Update();
    }
}

void Tile::Draw(SDL_FRect area)
{
    Texture tex;
    if (mBottomAni.IsInited())
        tex = mBottomAni.Get();
    else
        tex = TextureManager::Get(mTexId);
    Renderer::DrawTexture(tex, area);
    if (mTop == TOP)
    {
        if (mTopAni.IsInited())
            tex = mTopAni.Get();
        else if (mTexIdTop > 0)
            tex = TextureManager::Get(mTexIdTop);
        else
            return;
        Renderer::DrawTexture(tex, area);
    }
}

void Tile::DrawOnTop(SDL_FRect area)
{
    Texture tex;
    if (mTop == TOP_ONTOP)
    {
        if (mTopAni.IsInited())
            tex = mTopAni.Get();
        else if (mTexIdTop > 0)
            tex = TextureManager::Get(mTexIdTop);
        else
            return;
        Renderer::DrawTexture(tex, area);
    }
}

void Tile::Handle(const SDL_Event* e)
{

}

void Tile::SetTop(TopStatus status)
{
    mTop = status;
}

void Tile::SetSolid(bool solid)
{
    mIsSolid = solid;
}

void Tile::SetTopTexture(int textureId)
{
    mTexIdTop = textureId;
}

void Tile::SetBottomTexture(int textureId)
{
    mTexId = textureId;
}

int Tile::GetTopTexture() const
{
    return mTopAni.IsInited() ? mTopAni.GetTextureId() : mTexIdTop;
}

int Tile::GetBottomTexture() const
{
    return mBottomAni.IsInited() ? mBottomAni.GetTextureId() : mTexId;
}

bool Tile::IsSolid() const
{
    return mIsSolid;
}
