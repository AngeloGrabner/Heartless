#pragma once
#include "Scene.h"
#include <atomic>

#define GAME_PATH_TO_SAVES "../res/saves"
#define GAME_FILE_TYPE ".json"
#define GAME_WORLD_METADATA_FILE_NAME "init"

class SceneManager
{
	static bool sLoadErroFlag;
	static std::atomic_int sLoadStatusFlag;
	static std::string sPath;
	static std::string sWorldName;
	static std::string sFileType;
	static Scene* sSwapScene;
public:
	static void Handle(const SDL_Event* e);
	//loads the last loaded scene in the world
	static bool LoadWorld(const std::string& name);
	static bool UnloadWorld();

	static void LoadScene(const std::string& sceneName);
	static void UnloadScene(bool activeScene = false);

	static void WaitForLoad();
	static void WaitForUnload();

	static bool DoWhenSceneReady();

	static void CreateWorld(const std::string& name);
	static void CreateScene(const std::string& name, SDL_Point worldSizeInTiles);

private:
	static void InternLoad(const std::string& sceneName);
	static void InternUnload(const std::string& worldName);
};