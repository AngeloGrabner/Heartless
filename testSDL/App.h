#pragma once
#include <thread>
#include "Renderer.h"
#include "Window.h"
#include "Sound.h"
#include "scene/Scene.h"
#include "ui/Manager.h"

#include "scene/entity/controller/PlayerController.h"
#include "scene/entity/controller/SimpleEnemyController.h"
#include "scene/entity/Player.h"

#include "cereal/archives/json.hpp"
#include <fstream>


class App
{
private:
	bool mRunning = true;
	Timer mTimer = Timer("Global");
	Scene mScene;
	ui::Manager mUim;
	bool mDebugDraw = false;
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
		if (true)
		{	
			mScene = Scene({ 100,100 });
	
			auto& cam = mScene.GetCamera();
			cam.SetScale({ 4.0f,4.0f });

			std::shared_ptr<Creature> p = std::make_shared<Player>(SDL_FRect{ 48,48,16,16 }, 8, FtoMS(8.0f),8,8);
			p->AddActionController(std::make_shared<PlayerController>());
			p->SetStats(StatPack(1, 1, 30.0f));
			p->SetDrawBox(SDL_FRect(-2, -2, 4, 4));
			if (!mScene.InsertEntity(p))
			{
				SDL_assert(false);
			}

			for (int i = 0; i < 50; i++)
			{
				std::shared_ptr<Creature> p = std::make_shared<Creature>(SDL_FRect{ randomF(1600),randomF(1600),16,16}, 8, FtoMS(8.0f), 8, 8);
				p->AddActionController(std::make_shared<SimpleEnemyController>());
				p->SetStats(StatPack(1, 1, 20.0f));
				p->SetDrawBox(SDL_FRect(-2, -2, 4, 4));
				if (!mScene.InsertEntity(p))
				{
					SDL_assert(false);
				}
			}
		}
		else
		{
			try
			{
				std::ifstream ofs("data.json");
				cereal::JSONInputArchive arr(ofs);
				arr(mScene);
			}
			catch (std::exception& e)
			{
				LOG_PUSH(e.what());
				std::cout << e.what();
				return false;
			}
		}
		Scene::Set(&mScene);
		mScene.GetEntities()->PrintTree();
		Sound::WaitForLoad();
		return true;
	}
	void Run() 
	{
		
		while (mRunning)
		{
			mTimer.Start();
			Input::Update();

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				Input::EventHandler(&e);
				mScene.Handle(&e);
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
			mScene.Update();

			Renderer::SetColor(BLACK);
			Renderer::Clear();

			mScene.Draw();
			if (mDebugDraw)
				mScene.DebugDraw();

			mUim.Draw();
			if (mDebugDraw)
				mUim.DebugDraw();

			Renderer::UpdateScreen();
			Timer::SetGolbalDT(mTimer.Stop());
		}
	}
	~App()
	{
		try
		{
			std::ofstream ofs("data.json");
			cereal::JSONOutputArchive arr(ofs);
			arr(mScene);
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
			LOG_PUSH(e.what());
		}
		LOG_PRINT;
		Renderer::Free();
		Window::Free();
		SDL_Quit();
	}
};