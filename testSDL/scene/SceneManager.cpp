#include "SceneManager.h"

#include <thread>
#include <fstream>

#include "../Event.h"
#include "cereal/archives/json.hpp"

#include <filesystem>

#define NO_LOAD 0
#define LOADING 1
#define LOAD_FINISHED 2

bool SceneManager::sLoadErroFlag = false;
std::atomic_int SceneManager::sLoadStatusFlag = NO_LOAD;
std::string SceneManager::sPath = GAME_PATH_TO_SAVES;
std::string SceneManager::sFileType = GAME_FILE_TYPE;
std::string SceneManager::sWorldName;
Scene* SceneManager::sSwapScene = nullptr;


void SceneManager::Handle(const SDL_Event* e)
{
	std::string text;
	if (EventReceiver::StartSceneChange(e, text))
	{
		LoadScene(text);
	}
}

bool SceneManager::LoadWorld(const std::string& name)
{
	sWorldName = name;
	try
	{
		std::string sceneName;
		auto path = sPath + '/' + sWorldName + '/' + GAME_WORLD_METADATA_FILE_NAME;
		std::ifstream is(path);
		is >> sceneName;
		LoadScene(sceneName);
		return true;
	}
	catch (std::exception& e)
	{
		LOG_PUSH(e.what());
		SDL_assert(false);
		sWorldName.clear();
		return false;
	}
}

bool SceneManager::UnloadWorld()
{
	if (!sWorldName.empty())
	{
		if (Scene::Get())
		{
			std::ofstream os(sPath + '/' + sWorldName + '/' + GAME_WORLD_METADATA_FILE_NAME);
			os << Scene::Get()->mFileName;
			UnloadScene(true);
			sWorldName.clear();
			return true;
		}
		return false;
	}
	else
		SDL_assert(false);
	return false;
}

void SceneManager::LoadScene(const std::string& sceneName)
{
	if (!sWorldName.empty())
	{
		std::thread th(InternLoad, sceneName);
		th.detach();
	}
	else
		SDL_assert(false);
}

bool SceneManager::DoWhenSceneReady()
{
	if (sLoadStatusFlag == LOAD_FINISHED)
	{
		if (sLoadErroFlag)
		{
			SDL_assert(false);
			return false;
		}

		auto temp = Scene::GetMut();
		Scene::Set(sSwapScene);
		sSwapScene = temp;

		UnloadScene();

		EventBuilder::FinishedSceneChange();
		sLoadStatusFlag = NO_LOAD;
		return true;
	}
	return false;
}

void SceneManager::InternLoad(const std::string& sceneName)
{
	while (sLoadStatusFlag != NO_LOAD)
	{
		NO_OP;
	}
	Timer LoadTimer = Timer("SceneLoader Timer");
	sLoadErroFlag = false;
	sLoadStatusFlag = LOADING;
	sSwapScene = new Scene();
	if (sSwapScene)
	{
		try
		{
			auto path = (sPath + '/' + sWorldName + '/' + sceneName + sFileType);
			std::ifstream ofs(path);
			if (ofs.good())
			{
				cereal::JSONInputArchive arr(ofs);
				arr(*sSwapScene);
				sSwapScene->mFileName = sceneName;
			}
			else
			{
				sLoadErroFlag = true;
			}
		}
		catch (std::exception& e)
		{
			LOG_PUSH(e.what());
			sLoadErroFlag = true;
		}
	}
	else
	{
		sLoadErroFlag = true;
	}
	LoadTimer.Stop();
	std::cout << "InternLoad: " << LoadTimer.to_string() << "\n";
	sLoadStatusFlag = LOAD_FINISHED;
}

void SceneManager::UnloadScene(bool activeScene)
{
	if (!sWorldName.empty())
	{
		if (activeScene)
		{
			sSwapScene = Scene::GetMut();
			Scene::Set(nullptr);
		}
		if (sSwapScene)
		{
			std::thread th(InternUnload, sWorldName);
			th.detach();
		}
	}
	else
		SDL_assert(false);
}

void SceneManager::WaitForLoad()
{
	while (!SceneManager::DoWhenSceneReady())
	{
		NO_OP;
	}
}

void SceneManager::WaitForUnload()
{
	while (sLoadStatusFlag != LOAD_FINISHED)
	{
		NO_OP;
	}
}

void SceneManager::InternUnload(const std::string& worldName)
{
	while (sLoadStatusFlag != NO_LOAD)
	{
		NO_OP;
	}
	Timer UnloadTimer = Timer("SceneUnloader Timer");
	sLoadStatusFlag = LOADING;
	if (sSwapScene)
	{
		try
		{
			std::ofstream ofs((sPath + '/' + worldName + '/' + sSwapScene->mFileName + GAME_FILE_TYPE));
			if (ofs.good())
			{
				cereal::JSONOutputArchive arr(ofs);

				arr(*sSwapScene);
			}
			else 
				sLoadErroFlag = true;
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
			LOG_PUSH(e.what());
			sLoadErroFlag = true;
		}

		delete sSwapScene;
		sSwapScene = nullptr;
	}
	else 
		sLoadErroFlag = true;
	sLoadStatusFlag = LOAD_FINISHED;

	UnloadTimer.Stop();
	std::cout << "InternUnload: " << UnloadTimer.to_string() << "\n";
}

void SceneManager::CreateWorld(const std::string& name)
{
	if (std::filesystem::create_directory(std::filesystem::path(sPath + '/' + name)))
	{
		LOG_PUSH("unable to create new world");
		SDL_assert(false);
	}
}

void SceneManager::CreateScene(const std::string& name, SDL_Point  worldSizeInTiles)
{
	//todo
}


#undef NO_LOAD 
#undef LOADING 
#undef LOAD_FINISHED 