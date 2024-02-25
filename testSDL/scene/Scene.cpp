#include "Scene.h"
#include "../Renderer.h"
#include "../Event.h"
#include "../Window.h"

#include "entity/Player.h"

Scene* Scene::sScene = nullptr;	

#define TILE_SIZE 16

Scene::Scene(SDL_Point WorldSize)
	: mWorldSize(WorldSize)
{
	mCam = Camera(SDL_FPoint{ (float)WorldSize.x * TILE_SIZE,(float)WorldSize.y * TILE_SIZE });
	mTM = Tilemap(WorldSize.x, WorldSize.y, { 16.1f,16.1f });
	for (int y  =0; y < WorldSize.y; y++)
		for (int x = 0; x < WorldSize.x; x++)
		{
			if (x == 0 || y == 0 || (x == 2 && y == 2))
				mTM.Set(x, y, std::make_shared<Tile>(13*5+11, Tile::NO_TOP, -1, true));
			else 
				mTM.Set(x, y, std::make_shared<Tile>(11)); // hier
		}
	mDQT = std::make_shared<DQTE>(SDL_FRect{0.0f,0.0f,(float)mWorldSize.x * TILE_SIZE,(float)mWorldSize.y * TILE_SIZE },8);
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
	mCam.Handle(e);
	if (e->type >= SDL_USEREVENT)
	{
		InternHandle(e);
		if (mEditing)
			mEditor.Handle(e);
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
	
	//DebugDrawQt();
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
	DebugDrawQt();
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
			auto ptr = std::dynamic_pointer_cast<Player>(it->item);
			if (ptr)
			{
				mPlayer = ptr;
				return mPlayer;
			}
		}

		LOG_PUSH("Scene couln'd find player, therefore const GetPlayer returns nullptr");
		SDL_assert(false);

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
			auto ptr = std::dynamic_pointer_cast<Player>(it->item);
			if (ptr)
			{
				mPlayer = ptr;
				return mPlayer;
			}
		}

		LOG_PUSH("Scene couln'd find player, therefore GetPlayer returns nullptr");
		SDL_assert(false);

		return nullptr;
	}

	return mPlayer;
}

bool Scene::InsertEntity(std::shared_ptr<Entity> e)
{
	if (e)
	{
		return mDQT->Insert(e, e->AbsDrawBox());
	}
	return false;
}

const Scene* Scene::Get()
{
	return sScene;
}

Scene* Scene::GetMut()
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

void Scene::DebugDrawQt()
{
	Renderer::SetColor(YELLOW);

	std::list<SDL_FRect> rects = mDQT->GetRects(mCam.GetRect());
	for (auto r : rects)
	{
		Renderer::DrawRect(r);
	}
}






void Editor::Handle(const SDL_Event* e)
{
	std::string name;
	float sliderVal;
	std::string text;
	bool checkBoxVal;

	if (EventReceiver::Slider(e, name, sliderVal))
	{
		if (name == "editorLayerSlider")
		{
			if (sliderVal == 0)
			{
				mLayer = TILE_BOTTOM;
			}
			else if (sliderVal == .5)
			{
				mLayer = ENTITY;
			}
			else if (sliderVal == 1)
			{
				mLayer = TILE_TOP;
			}
			else
			{
				SDL_assert(false);
			}
		}
	}
	else if (EventReceiver::Select(e, name, text))
	{
		static int row = 0, colum = 0;
		static Tile::TopStatus top = Tile::NO_TOP;
		bool flag = false;
		if (name == "editorTileTextureSelect")
		{
			flag = true;
			if (text == "gras")
			{
				row = 0;
			}
			else if (text == "sand")
			{
				row = 1;
			}
			else if (text == "dirt")
			{
				row = 2;
			}
			else if (text == "snow")
			{
				row = 3;
			}
			else if (text == "stone brick w")
			{
				row = 4;
			}
			else if (text == "dirt w")
			{
				row = 5;
			}
			else if (text == "stone w")
			{
				row = 6;
			}
			else
			{
				SDL_assert(false); //unknown Texture
			}


		}
		else if (name == "editorTileSubTextureSelect")
		{
			flag = true;
			if (text == "full")
			{
				colum = 0;
			}
			else if (text == "left")
			{
				colum = 1;
			}
			else if (text == "right")
			{
				colum = 2;
			}
			else if (text == "bottom")
			{
				colum = 3;
			}
			else if (text == "top")
			{
				colum = 4;
			}
			else if (text == "inner t l")
			{
				colum = 5;
			}
			else if (text == "inner t r")
			{
				colum = 6;
			}
			else if (text == "inner b l")
			{
				colum = 7;
			}
			else if (text == "inner b r")
			{
				colum = 8;
			}
			else if (text == "outer t l")
			{
				colum = 9;
			}
			else if (text == "outer t r")
			{
				colum = 10;
			}
			else if (text == "outer b l")
			{
				colum = 11;
			}
			else if (text == "outer b r")
			{
				colum = 12;
			}
			else
			{
				SDL_assert(false); //unknown Texture
			}
		}
		else if (name == "editorTileTopSelect")
		{

			if (text == "No Top")
			{
				top = Tile::NO_TOP;
			}
			else if (text == "Top")
			{
				top = Tile::TOP;
			}
			else if (text == "Top on Top")
			{
				top = Tile::TOP_ONTOP;
			}
			else
			{
				SDL_assert(false); //unkonw tile top... type.. whatever
			}


			for (auto ptr : mSelectedTiles)
			{
				ptr->SetTop(top);
			}
		}
		else if (name == "editorEntityStatsSelect")
		{
			mSelectedEntityStat = text;
		}
		if (flag)
		{
			constexpr int textureTypes = 13, offset = 11; // see data.csv for details
			if (mLayer == TILE_BOTTOM)
			{
				if (top == Tile::NO_TOP)
				{
					for (auto ptr : mSelectedTiles)
					{
						ptr->SetBottomTexture(row * textureTypes + colum + offset);
					}
				}
				else if (top == Tile::TOP)
				{
					for (auto ptr : mSelectedTiles)
					{
						ptr->SetTopTexture(row * textureTypes + colum + offset);
					}
				}
			}
			else if (mLayer == TILE_TOP)
			{
				for (auto ptr : mSelectedTiles)
				{
					ptr->SetTopTexture(row * textureTypes + colum + offset);
				}
			}
		}
	}
	else if (EventReceiver::CheckBox(e, name, checkBoxVal))
	{
		if (name == "editorTileSolidCheckbox")
		{
			for (auto ptr : mSelectedTiles)
			{
				ptr->SetSolid(checkBoxVal);
			}
		}
	}
	else if (EventReceiver::InputField(e, name, text))
	{
		if (name == "editorEntityXInputField")
		{
			float offx = 0;
			try
			{
				offx = std::stof(text);
			}
			catch (std::exception& e) 
			{
				EventBuilder::ReInputField("editorEntityXInputField","NaN");
				LOG_PUSH(e.what());
			}
			for (auto& en : mSelectedEntities)
			{
				en.data->item->HitBox().x += offx;
			}
			SetSingleEntityUiData();
		}
		else if (name == "editorEntityYInputField")
		{
			float offy = 0;
			try
			{
				offy = std::stof(text);
			}
			catch (std::exception& e)
			{
				EventBuilder::ReInputField("editorEntityYInputField", "NaN");
				LOG_PUSH(e.what());
			}
			for (auto& en : mSelectedEntities)
			{
				en.data->item->HitBox().y += offy;
			}
			SetSingleEntityUiData();
		}
		else if (name == "editorEntityStatsInputField")
		{
			try
			{
				StatPack sp;
				for (auto& enttPtr : mSelectedEntities)
				{
					sp = enttPtr.data->item->GetStats();
					if (mSelectedEntityStat == "atk")
					{
						sp.atk = (int)std::round(std::stof(text));
					}
					else if (mSelectedEntityStat == "hp")
					{
						sp.hp = (int)std::round(std::stof(text));
					}
					else if (mSelectedEntityStat == "spd")
					{
						sp.spd = std::round(std::stof(text));
					}
					else
						SDL_assert(false); // unsupported stats
					enttPtr.data->item->SetStats(sp);
				}
				SetSingleEntityUiData();
			}
			catch (std::exception& e)
			{
				EventBuilder::ReInputField("editorEntityStatsInputField","NaN");
				LOG_PUSH(e.what());
			}
			
		}
	}
}

void Editor::Update(Scene* scene)
{
	DoCamDrag(scene);
	DoSelceting(scene);
}

void Editor::ClearSelection()
{
	mSelectionArea.x = -1;
	mSelectionArea.y = -1;
	mSelectedTiles.clear();
	mSelectedEntities.clear();
}

void Editor::DoCamDrag(Scene* scene)
{
	if (Input::GetMouse(Input::LMB,Input::EDITOR).Hold())
	{
		//DB_OUT("editorDrag");
		auto pos = Input::GetRelativeMousePos();
		auto scale = Scene::Get()->GetCamera().GetScale();
		scene->GetCamera().SetPosRel(SDL_FPoint{ -(pos.x / scale.x),-(pos.y / scale.y) });
	}
	if (Input::GetWheel().y < 0)
	{
		scene->GetCamera().SetScaleRel(SDL_FPoint{ 0.5,0.5 });
	}
	else if (Input::GetWheel().y > 0)
	{
		scene->GetCamera().SetScaleRel(SDL_FPoint{ 2.0,2.0 });
	}
}

void Editor::DoSelceting(Scene* scene)
{
	//reset selection
	if (Input::GetMouse(Input::MMB, Input::EDITOR).Down())
	{
		ClearSelection();
		DB_OUT("reset");
	}

	//select 
	if (mSelecting)
	{
		auto pos = scene->ScreenToWorld(Input::GetMousePos());
		mSelectionArea.w = pos.x - mSelectionArea.x;
		mSelectionArea.h = pos.y - mSelectionArea.y;
	}
	if (Input::GetMouse(Input::RMB, Input::EDITOR).Up() && mSelecting)
	{
		mSelecting = false;

		if (mSelectionArea.x >= 0 && mSelectionArea.y >= 0)
		{
			if (mLayer == TILE_BOTTOM || mLayer == TILE_TOP)
			{
				scene->mTM.Get(mSelectionArea, mSelectTileSize, mSelectedTiles);
				//std::cout << mSelectedTiles.size() << std::endl;

				DB_OUT("seledcted");
			}
			else if (mLayer == ENTITY)
			{
				mSelectedEntities = scene->mDQT->Search(mSelectionArea);
			}
		}
	}
	else if (Input::GetMouse(Input::RMB,Input::EDITOR).Down())
	{
		ClearSelection();
		mSelecting = true;
		auto pos = scene->ScreenToWorld(Input::GetMousePos());
		mSelectionArea.x = pos.x;
		mSelectionArea.y = pos.y;
	}
}

void Editor::SetSingleEntityUiData()
{
	if (mSelectedEntities.size() == 1)
	{
		auto x = std::to_string(mSelectedEntities.front().data->item->GetHitBox().x);
		auto y = std::to_string(mSelectedEntities.front().data->item->GetHitBox().y);
		EventBuilder::ReInputField("editorEntityXInputField", x);
		EventBuilder::ReInputField("editorEntityYInputField", y);

		std::string text;
		if (mSelectedEntityStat == "atk")
		{
			text = std::to_string(mSelectedEntities.front().data->item->GetStats().atk);
		}
		else if (mSelectedEntityStat == "hp")
		{
			text = std::to_string(mSelectedEntities.front().data->item->GetStats().hp);
		}
		else if (mSelectedEntityStat == "spd")
		{
			text = std::to_string(mSelectedEntities.front().data->item->GetStats().spd);
		}
		else
		{
			SDL_assert(false); // unsupported stat
			text = "error";
		}
		EventBuilder::ReInputField("editorEntityStatsInputField", text);
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
		DB_OUT("unknown editor layer");
		SDL_assert(false);
		break;
	}
	if (Input::GetKey(SDLK_SPACE, Input::EDITOR).DownOrHold())
	{
		scene->mTM.DebugDraw(area);
		auto entt = scene->mDQT->Search(area);
		for (auto& en : entt)
			en.data->item->DebugDraw();
		scene->DebugDrawQt();
	}
	if (mSelecting || !mSelectedEntities.empty() || !mSelectedTiles.empty())
	{
		Renderer::SetColor(WHITE);
		Renderer::DrawRect(mSelectionArea);
	}
}
