#include "Scene.h"
#include "../Renderer.h"
#include "../Event.h"
#include "../Window.h"

#include "entity/Player.h"

Scene* Scene::sScene = nullptr;	

Scene::Scene(SDL_Point WorldSize)
	: mWorldSize(WorldSize)
{
	mTM = Tilemap(WorldSize.x, WorldSize.y, { 16,16 });
	for (int y  =0; y < WorldSize.y; y++)
		for (int x = 0; x < WorldSize.x; x++)
		{
			if (x == 0 || y == 0 || (x == 2 && y == 2))
				mTM.Set(x, y, std::make_shared<Tile>(9, Tile::TOP_ONTOP, 10, true));
			else 
				mTM.Set(x, y, std::make_shared<Tile>(7));
		}
	mDQT = std::make_shared<DQTE>(SDL_FRect{0.0f,0.0f,(float)mWorldSize.x,(float)mWorldSize.y},8);
}

Scene::Scene(Scene&& other) noexcept
{
	std::swap(mDQT,other.mDQT);
	std::swap(mPlayer, other.mPlayer);
	std::swap(mTM, other.mTM);
	mCam = other.mCam;
	mWorldSize = other.mWorldSize;
}

Scene& Scene::operator=(Scene&& other) noexcept
{
	if (this != &other)
	{
		std::swap(mDQT, other.mDQT);
		std::swap(mPlayer, other.mPlayer);
		std::swap(mTM, other.mTM);
		mCam = other.mCam;
		mWorldSize = other.mWorldSize;
	}
	return *this;
}

void Scene::Handle(const SDL_Event* e)
{
	if (e->type >= SDL_USEREVENT)
	{
		InternHandle(e);
		mCam.Handle(e);
		auto area = mCam.GetRect();
		mTM.Handle(e, area);
		auto ent = mDQT->Search(area);
		for (auto& en : ent)
		{
			en.data->item->Handle(e);
		}
	}
}

void Scene::Update()
{
	mCam.Update();
	if (mEditing)
	{
		mEditor.Update(this);
		return;
	}

	auto area = mCam.GetRect();
	auto ent = mDQT->Search(area);
	for (auto& en : ent)
	{
		if (!en.data->item->Update())
		{
			mDQT->Remove(en.data);
		}
		else
		{
			SDL_FRect pos = en.data->item->AbsDrawBox();
			if (pos.x != en.rect.x || pos.y != en.rect.y)
			{
				mDQT->Relocate(en.data, pos);
			}
		}
	}
	mTM.Update(area);

}

void Scene::Draw()
{
	if (mEditing)
	{
		mEditor.Draw(this);
		return;
	}

	auto area = mCam.GetRect();
	auto ent = mDQT->Search(area);
	mTM.Draw(area);
	for (auto& en : ent)
	{
		en.data->item->Draw();
	}
	mTM.DrawOnTop(area);
}

void Scene::DebugDraw()
{
	auto area = mCam.GetRect();
	auto ent = mDQT->Search(area);
	mTM.DebugDraw(area);
	for (auto& en : ent)
	{
		en.data->item->DebugDraw();
	}
	Renderer::SetColor(CYAN);
	Renderer::DrawRect(mCam.GetRect());
}

const Camera& Scene::GetCamera() const
{
	return mCam;
}

SDL_FPoint Scene::ScreenToWorld(SDL_Point pos)
{
	SDL_FPoint POS;
	auto scale = mCam.GetScale();
	POS.x = pos.x / scale.x;
	POS.y = pos.y / scale.y;
	auto ppos = mCam.GetPos();
	POS.x += ppos.x;
	POS.y += ppos.y;
	return POS;
}

const Tilemap& Scene::GetTileMap() const
{
	return mTM;
}

const std::shared_ptr<Scene::DQTE> Scene::GetEntities() const
{
	return mDQT;
}

const std::shared_ptr<Player> Scene::GetPlayer() const
{
	if (!mPlayer)
	{
		for (auto it = mDQT->begin(); it != mDQT->end(); it++)
		{
			if (it->item->Type == Player::Type)
			{
				mPlayer = std::dynamic_pointer_cast<Player>(it->item);
				return mPlayer;
			}
		}
#if _DEBUG
		LOG("Scene couln'd find player, therefore const GetPlayer returns nullptr");
		throw;
#endif
		return nullptr;
	}

	return mPlayer;
}

Camera& Scene::GetCamera()
{
	//__debugbreak();
	return mCam;
}

Tilemap& Scene::GetTileMap()
{
	//__debugbreak();
	return mTM;
}

std::shared_ptr<Scene::DQTE> Scene::GetEntities()
{
	//__debugbreak();
	return mDQT;
}

std::shared_ptr<Player> Scene::GetPlayer()
{
	//__debugbreak();
	if (!mPlayer)
	{
		for (auto it = mDQT->begin(); it != mDQT->end(); it++)
		{
			if (it->item->Type == Player::Type)
			{
				mPlayer = std::static_pointer_cast<Player>(it->item);
				return mPlayer;
			}
		}
#if _DEBUG
		LOG("Scene couln'd find player, therefore GetPlayer returns nullptr");
		throw;
#endif
		return nullptr;
	}
		
	return mPlayer;
}

bool Scene::InsertEntity(std::shared_ptr<Entity> e)
{
	if (e)
	{
		return mDQT->Insert(e, e->GetHitBox());
	}
	return false;
}

const Scene* Scene::Get()
{
	return sScene;
}

void Scene::Set(Scene* instance)
{
	sScene = instance;
}

void Scene::InternHandle(const SDL_Event* e)
{
	std::string uiName;
	bool checked = false;
	if (EventReceiver::CheckBox(e, uiName, checked))
	{
		if (uiName == "editorCheckBox")
		{
			mEditing = checked;
		}
	}
	
}





void Editor::Update(Scene* scene)
{
	DoCamDrag();
	DoSelceting(scene);
}

void Editor::ClearSelection()
{
	mSelectionArea.x = -1;
	mSelectionArea.y = -1;
	mSelectedTiles.clear();
	mSelectedEntities.clear();
}

void Editor::DoCamDrag()
{
	if (Input::GetMouse(Input::LMB).Hold())
	{
		std::cout << "editorDrag\n";
		auto pos = Input::GetRelativeMousePos();
		auto scale = Scene::Get()->GetCamera().GetScale();
		EventBuilder::CameraRelPos(SDL_FPoint{ -(pos.x / scale.x),-(pos.y / scale.y) });
	}
	if (Input::GetWheel().y < 0)
	{
		EventBuilder::CameraScaleRel(SDL_FPoint{ 0.8,0.8 });
	}
	else if (Input::GetWheel().y > 0)
	{
		EventBuilder::CameraScaleRel(SDL_FPoint{ 1.2,1.2 });
	}
}

void Editor::DoSelceting(Scene* scene)
{
	//reset selection
	if (Input::GetMouse(Input::MMB).Down())
	{
		ClearSelection();
		std::cout << "reset\n";
	}

	//select 
	if (mSelecting)
	{
		auto pos = scene->ScreenToWorld(Input::GetMousePos());
		mSelectionArea.w = pos.x - mSelectionArea.x;
		mSelectionArea.h = pos.y - mSelectionArea.y;
	}
	if (Input::GetMouse(Input::RMB).Up() && mSelecting)
	{
		mSelecting = false;

		if (mSelectionArea.x >= 0 && mSelectionArea.y >= 0)
		{
			if (mLayer == TILE_BOTTOM || mLayer == TILE_TOP)
			{
				scene->mTM.Get(mSelectionArea, mSelectTileSize, mSelectedTiles);
				std::cout << mSelectedTiles.size() << std::endl;

				std::cout << "seledcted\n";
			}
			else if (mLayer == ENTITY)
			{
				mSelectedEntities = scene->mDQT->Search(mSelectionArea);
			}
		}
	}
	else if (Input::GetMouse(Input::RMB).Down())
	{
		ClearSelection();
		mSelecting = true;
		auto pos = scene->ScreenToWorld(Input::GetMousePos());
		mSelectionArea.x = pos.x;
		mSelectionArea.y = pos.y;
	}
}

void Editor::Draw(Scene* scene)
{	
	auto area = scene->mCam.GetRect();
	switch (mLayer)
	{
	case Editor::TILE_BOTTOM:
		scene->mTM.Draw(area);
		if (!mSelectedTiles.empty() || mSelecting)
			scene->mTM.DebugDraw(mSelectionArea);
		break;
	case Editor::ENTITY:
	{
		auto ent = scene->mDQT->Search(area);
		for (auto& en : ent)
		{
			en.data->item->Draw();
		}
		if (!mSelectedEntities.empty() || mSelecting)
		for (auto& en : mSelectedEntities)
		{
			en.data->item->DebugDraw();
		}
	}
		break;
	case Editor::TILE_TOP:
		scene->mTM.DrawOnTop(area);
		if (!mSelectedTiles.empty() || mSelecting)
			scene->mTM.DebugDraw(mSelectionArea);
		break;
	case Editor::LAST_LAYER:
	default:
		std::cout << "unknown editor layer";
		throw;
	}
	if (mSelecting || !mSelectedEntities.empty() || !mSelectedTiles.empty())
	{
		Renderer::SetColor(WHITE);
		Renderer::DrawRect(mSelectionArea);
	}
}
