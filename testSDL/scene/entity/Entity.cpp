#include "Entity.h"
#include "../../Renderer.h"

#include "../Scene.h"

size_t Entity::sIdCounter = Entity::InvalidId;

Entity::Entity(SDL_FRect hitbox, int textureId, SDL_FRect drawBoxOffset)
	: mHitBox(hitbox), mTexId(textureId), mDrawBox(drawBoxOffset)
{
	mId = ++sIdCounter;
	mFlags.reset();
	mFlags[ALIVE] = true;
}

Entity::Entity(SDL_FRect hitbox, int textureID, Milliseconds frameDuation,
	unsigned int frameCount, unsigned int animationCount)
	: mHitBox(hitbox), mAni(std::make_unique<Animation>(textureID,frameDuation,frameCount,animationCount))
{
	mId = ++sIdCounter;
	mFlags.reset();
	mFlags[ALIVE] = true;
}

Entity::Entity(Entity&& other) noexcept
{
	mDrawBox = other.mDrawBox;
	mHitBox = other.mHitBox;
	mStats = other.mStats;
	mDir = other.mDir;
	mId = other.mId;
	mFlags = std::move(other.mFlags);
	mTexId = other.mTexId;
	mAni.swap(other.mAni);
}

Entity& Entity::operator=(Entity&& other) noexcept
{
	if (this != &other)
	{
		mDrawBox = other.mDrawBox;
		mHitBox = other.mHitBox;
		mStats = other.mStats;
		mDir = other.mDir;
		mId = other.mId;
		mFlags = std::move(other.mFlags);
		mTexId = other.mTexId;
		mAni.swap(other.mAni);
	}
	return *this;
}

void Entity::Handle(const SDL_Event* e)
{
	//hier
}

void Entity::Draw()
{
	Texture tex;
	if (mAni)
	{
		tex = mAni->Get();
		if (!tex.tex)
			Scene::Get()->GetEntities()->PrintTree();
		SDL_assert(tex.tex);
	}
	else
	{
		tex = TextureManager::Get(mTexId);
	}
	Renderer::DrawTexture(tex, AbsDrawBox());
}

void Entity::DebugDraw()
{
	Renderer::SetColor(RED);
	Renderer::DrawRect(mHitBox);
	Renderer::SetColor(GREEN);
	Renderer::DrawRect(AbsDrawBox());
}

SDL_FRect Entity::AbsDrawBox() const
{
	return {
		mHitBox.x + mDrawBox.x,
		mHitBox.y + mDrawBox.y,
		mHitBox.w + mDrawBox.w,
		mHitBox.h + mDrawBox.h
	};
}

SDL_FRect Entity::GetHitBox() const
{
	return mHitBox;
}

SDL_FRect& Entity::HitBox()
{
	return mHitBox;
}

Entity* Entity::SetStats(const StatPack& stats)
{
	mStats = stats;
	return this;
}

StatPack Entity::GetStats() const
{
	return mStats;
}


bool Entity::GetFlag(Flags f) const
{
	return mFlags[f];
}

Entity* Entity::SetFlag(Flags f, bool value)
{
	mFlags[f] = value;
	return this;
}

Entity* Entity::SetAnimation(std::unique_ptr<Animation> ani)
{
	mAni.swap(ani);
	return this;
}

Entity* Entity::SetDrawBox(SDL_FRect offsetbox)
{
	mDrawBox = offsetbox;
	return this;
}

SDL_FRect Entity::GetDrawBox() const
{
	return mDrawBox;
}

#include "../Scene.h"

SDL_FRect Collide(SDL_FRect pos)
{
	std::vector<SDL_FRect> tilesAreas;
	Scene::Get()->GetTileMap().GetSolidAreas(pos,tilesAreas);
	for (auto& area : tilesAreas)
	{		
		float distanceX = std::abs(pos.x - area.x);
		float distanceY = std::abs(pos.y - area.y);


		float minDistanceX, minDistanceY;
		if (pos.x < area.x)
		{
			minDistanceX= pos.w;
		}
		else
		{
			minDistanceX = area.w;
		}

		if (pos.y < area.y)
		{
			minDistanceY = pos.h;
		}
		else
		{
			minDistanceY = area.h;
		}



		if (distanceX <= minDistanceX && distanceY <= minDistanceY)
		{
			float offsetX = minDistanceX - distanceX;
			float offsetY = minDistanceY - distanceY;
			if (offsetX < offsetY)
			{
				if (pos.x < area.x)
				{
					pos.x -= offsetX;
				}
				else
				{
					pos.x += offsetX;
				}
			}
			else
			{
				if (pos.y < area.y)
				{
					pos.y -= offsetY;
				}
				else
				{
					pos.y += offsetY;
				}
			}
		}
	}
	
	return pos;
}
