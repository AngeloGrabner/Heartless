#pragma once
#include <cereal/types/memory.hpp>
#include "../sdl_type_serializer.h"
#include "../Quadtree.h"
#include "tilemap/Tilemap.h"
#include "entity/Entity.h"
#include "Camera.h"
#include "../Input.h"

static constexpr size_t QT_SIZE = 10000;

class Player;

class Scene;

class Editor
{
	enum Layer
	{
		TILE_BOTTOM,
		ENTITY,
		TILE_TOP,
		LAST_LAYER
	};
	Layer mLayer = TILE_BOTTOM;

	SDL_FRect mSelectionArea = { -1,-1,0,0 };
	SDL_Point mSelectTileSize = { 0,0 };
	std::vector<std::shared_ptr<Tile>> mSelectedTiles;
	using QT_List = std::list<Interbundle<DynamicQuadTree<std::shared_ptr<Entity>, SDL_FRect, QT_SIZE>::ITER, SDL_FRect>>;
	QT_List mSelectedEntities;
	bool mSelecting = false;

public:
	void Handle(const SDL_Event* e);
	void Update(Scene* scene);
	void ClearSelection();
	void DoCamDrag();
	void DoSelceting(Scene* scene);
	void Draw(Scene* scene);
};

class Scene
{
private:
	friend Editor;
	static Scene* sScene;

	using DQTE = DynamicQuadTree<std::shared_ptr<Entity>, SDL_FRect, QT_SIZE>;

	size_t mWorldId = 0; // <-- todo 
	std::shared_ptr<DQTE> mDQT = nullptr;
	mutable std::shared_ptr<Player> mPlayer = nullptr;
	Tilemap mTM;
	Camera mCam;
	SDL_Point mWorldSize = { -1,-1 };
	bool mEditing = false;
	Editor mEditor;
public:
	Scene() = default;
	Scene(SDL_Point WorldSize);
	//Scene(const Scene& other);
	Scene(Scene&& other) noexcept;
	//Scene& operator=(const Scene& other);
	Scene& operator=(Scene&& other) noexcept;

	void Handle(const SDL_Event* e);
	void Update();
	void Draw();
	void DebugDraw();

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);

	SDL_FPoint ScreenToWorld(SDL_Point pos);

	const Camera& GetCamera() const;
	const Tilemap& GetTileMap() const;
	const std::shared_ptr<DQTE> GetEntities() const;
	const std::shared_ptr<Player> GetPlayer() const;

	Camera& GetCamera();
	Tilemap& GetTileMap();
	std::shared_ptr<DQTE> GetEntities();
	std::shared_ptr<Player> GetPlayer();

	bool InsertEntity(std::shared_ptr<Entity> e);

	static const Scene* Get();
	static void Set(Scene* instance);
private:
	void InternHandle(const SDL_Event* e);

};

template<class Archive>
inline void Scene::save(Archive& ar) const
{
	ar(cereal::make_nvp("worldSize", mWorldSize),
		cereal::make_nvp("Editing",mEditing),
		cereal::make_nvp("DQT",mDQT),
		cereal::make_nvp("TM",mTM),
		cereal::make_nvp("worldId",mWorldId),
		cereal::make_nvp("cam",mCam));
}

template<class Archive>
inline void Scene::load(Archive& ar)
{
	ar(mWorldSize, mEditing, mDQT, mTM, mWorldId, mCam);
}
