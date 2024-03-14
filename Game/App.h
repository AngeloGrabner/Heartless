#pragma once
#include <thread>
#include "Renderer.h"
#include "Window.h"
#include "Sound.h"
#include "scene/SceneManager.h"
#include "ui/Manager.h"

#include "scene/entity/controller/PlayerController.h"
#include "scene/entity/controller/SimpleEnemyController.h"
#include "scene/entity/Player.h"


#include "cereal/archives/json.hpp"
#include <fstream>

#define USE_SCENEMANAGER 

class App
{
private:
	bool mRunning = true;
	Timer mTimer = Timer("Global");
	ui::Manager mUim;
	bool mDebugDraw = true;
public:
	App()
	{
		SDLCHECK(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO));

		Window::Init("SDL Test", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1280,720});
		Renderer::Init(Window::Get());
		TextureManager::Init("../res/textures.csv");
		Sound::Init("../res/sounds.csv");
		ui::FontWriter::Init("../res/font.csv");
		Input::Init();
		Event::Init();
		mUim.Init();
		
		Window::SetIcon("../res/heart.bmp");

		Renderer::SetScreenMode(false);
		Renderer::SetVSync(Renderer::VSYNC_DISABLE);

		srand((unsigned int)(SDL_GetTicks64() * (SHRT_MAX - 123)) + 3);
	}
	bool Init()
	{
#ifndef  USE_SCENEMANAGER 
			if (true)
			{
				Scene* scene = new Scene({ 100,100 });
				auto& cam = scene->GetCamera();
				cam.SetScale({ 4.0f,4.0f });

				std::shared_ptr<Creature> p = std::make_shared<Player>(SDL_FRect{ 48,48,16,16 }, 8, FtoMS(8.0f), 8, 8);
				p->AddActionController(std::make_shared<PlayerController>());
				p->SetStats(StatPack(1, 1, 200.0f));
				p->SetDrawBox(SDL_FRect(-2, -2, 4, 4));
				if (!scene->InsertEntity(p))
				{
					SDL_assert(false);
				}

				for (int i = 0; i < 200; i++)
				{
					std::shared_ptr<Creature> p = std::make_shared<Creature>(SDL_FRect{ randomF(1600),randomF(1600),16,16 }, 8, FtoMS(8.0f), 8, 8);
					p->AddActionController(std::make_shared<SimpleEnemyController>());
					p->SetStats(StatPack(1, 1, 20.0f));
					p->SetDrawBox(SDL_FRect(-2, -2, 4, 4));
					if (!scene->InsertEntity(p))
					{
						SDL_assert(false);
					}
				}
				Scene::Set(scene);
			}
			else
			{
				std::ifstream ifs("../res/saves/world1/data.json");
				cereal::JSONInputArchive arr(ifs);
				Scene* temp = new Scene();
				arr(*temp);
				Scene::Set(temp);
			}
#else
		SceneManager::LoadWorld("world1");
		SceneManager::WaitForLoad();
#endif 
		Sound::WaitForLoad();

		return true;
	}
	void Run() 
	{
		SDL_Event e;
		while (mRunning)
		{
			mTimer.Start();

			Input::Update();
			SceneManager::DoWhenSceneReady();

			while (SDL_PollEvent(&e))
			{
				Input::EventHandler(&e);
				SceneManager::Handle(&e);
				Scene::GetMut()->Handle(&e);
				mUim.Handle(&e);
				if  (e.type == SDL_QUIT)
				{
					mRunning = false;
				}
				if (e.type >= SDL_USEREVENT && e.type < SDL_LASTEVENT)
				{
					UserEventDeallocator(&e);
				}
			}

			mUim.Update();
			Scene::GetMut()->Update();

			Renderer::SetColor(BLACK);
			Renderer::Clear();

			Scene::GetMut()->Draw();
			if (mDebugDraw)
				Scene::GetMut()->DebugDraw();

			mUim.Draw();
			if (mDebugDraw)
				mUim.DebugDraw();

			Renderer::UpdateScreen();
			Timer::SetGolbalDT(mTimer.Stop());
		}
	}
	~App()
	{
#ifndef USE_SCENEMANAGER
		std::ofstream ofs("../res/saves/world1/data.json");
		cereal::JSONOutputArchive arr(ofs);
		
		arr(*Scene::GetMut());
		delete Scene::GetMut();
		Scene::Set(nullptr);
#else
		SceneManager::UnloadWorld();
		SceneManager::WaitForUnload();
#endif 
		Renderer::Free();
		Window::Free();
		LOG_PRINT;
		SDL_Quit();
	}
};