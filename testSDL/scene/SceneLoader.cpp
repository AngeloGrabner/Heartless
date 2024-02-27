#include "SceneLoader.h"

#include <thread>
#include <fstream>

#include "../Event.h"
#include "cereal/archives/json.hpp"



bool SceneManager::Init(const std::string& worldName)
{
	sPath = GAME_PATH_TO_SAVES;
	sFileType = GAME_FILE_TYPE;
	sWorldName = worldName;
	return LoadWorldMetaData();
}

void SceneManager::Handle(const SDL_Event* e)
{
	std::string text;
	if (EventReceiver::StartSceneChange(e, text))
	{
		LoadScene(text);
	}
}

bool SceneManager::LoadWorldMetaData()
{
	try
	{
		std::ifstream is(sPath + '/' + sWorldName + '/' + GAME_WORLD_METADATA_FILE_NAME);
		is >> // hier
		return true;
	}
	catch (std::exception& e)
	{
		LOG_PUSH(e.what());
		SDL_assert(false);
		return false;
	}
}

void SceneManager::LoadScene(const std::string& sceneName)
{
	std::thread th(InternLoad, sceneName);
	th.detach();
}

void SceneManager::DoWhenSceneReady()
{
	if (sLoadFinishedFlag)
	{
		if (sLoadErroFlag)
		{
			SDL_assert(false);
			return;
		}

		auto temp = Scene::GetMut();
		Scene::Set(sSwapScene);
		sSwapScene = temp;

		InternUnload();

		EventBuilder::FinishedSceneChange();
	}
}

void SceneManager::InternLoad(const std::string& sceneName)
{
	while (!sLoadFinishedFlag)
	{

	}
	Timer LoadTimer = Timer("SceneLoader Timer");
	sLoadErroFlag = false;
	sLoadFinishedFlag = false;
	sSwapScene = new Scene();
	if (sSwapScene)
	{
		try
		{
			std::ifstream ofs((sPath + '/' + sWorldName + '/' + sceneName + sFileType));
			cereal::JSONInputArchive arr(ofs);
			arr(*sSwapScene);
			sSwapScene->mFileName = sceneName + sFileType;
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
	sLoadFinishedFlag = true;
}

void SceneManager::UnloadScene(bool activeScene)
{
	if (activeScene)
	{
		sSwapScene = Scene::GetMut();
		Scene::Set(nullptr);
	}
	if (sSwapScene)
	{
		std::thread th(InternUnload);
		th.detach();
	}
}

void SceneManager::InternUnload()
{
	while (!sLoadFinishedFlag)
	{

	}
	Timer UnloadTimer = Timer("SceneUnloader Timer");
	sLoadFinishedFlag = false;
	try
	{
		std::ofstream ofs((sPath + '/' + sWorldName + '/' + sSwapScene->mFileName));
		cereal::JSONOutputArchive arr(ofs);
		arr(*Scene::GetMut());
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
		LOG_PUSH(e.what());
	}

	delete sSwapScene;
	sSwapScene = nullptr;

	sLoadFinishedFlag = true;

	UnloadTimer.Stop();
	std::cout << "InternUnload: " << UnloadTimer.to_string() << "\n";
}
