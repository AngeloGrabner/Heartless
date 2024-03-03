#include "SceneSwapTile.h"
#include "../Scene.h"
#include "../entity/Entity.h"
#include "../entity/Player.h"
#include "../../Event.h"


SceneSwapTile::SceneSwapTile(int bottomTextureId, TopStatus ts, int topTextureId, bool solid, uint8_t light)
	: Tile(bottomTextureId,ts,topTextureId,solid,light)
{
}

SceneSwapTile* SceneSwapTile::SetDestName(const std::string& fileNameWithoutFileExtension)
{
	mFileName = fileNameWithoutFileExtension;
	return this;
}

SceneSwapTile* SceneSwapTile::SetDestPos(SDL_FPoint destPos)
{
	mDest = destPos;
	return this;
}

void SceneSwapTile::Update(SDL_FRect area)
{
	Tile::Update(area);
	mTransferList.clear();
	auto entt = Scene::Get()->GetEntities()->Search(area);
	for (auto en : entt)
	{
		if (!en.data->item->GetFlag(Entity::Flags::TRIVIALLY_DESPAWNABLE))
		{
			mTransferList.push_back(en.data->item);
		}
		if (std::dynamic_pointer_cast<Player>(en.data->item))
		{
			EventBuilder::StartSceneChange(mFileName, mDest, mTransferList);
		}
	}
}

bool SceneSwapTile::Comp(const Tile& other)
{
	if (this == &other)
		return true;
	auto o = dynamic_cast<const SceneSwapTile&>(other);
	if (&o != nullptr)
	{
		if (mDest.x == o.mDest.x &&
			mDest.y == o.mDest.y &&
			mFileName == o.mFileName)
		{
			return (Tile::Comp(other));
		}
	}
	return false;
}
