#pragma once
#include "Scene.h"
#include <atomic>

#define GAME_PATH_TO_SAVES "../res/saves"
#define GAME_FILE_TYPE ".json"
#define GAME_WORLD_METADATA_FILE_NAME "init"

class SceneManager
{
	static bool sLoadErroFlag;
	static std::atomic_bool sLoadFinishedFlag;
	static std::string sPath;
	static std::string sWorldName;
	static std::string sFileType;
	static std::string sInitLoadSceneName;
	static Scene* sSwapScene;
public:
	static bool Init(const std::string& worldName);
	static void Handle(const SDL_Event* e);
	static bool LoadWorldMetaData();
	static void LoadScene(const std::string& sceneName);
	static void UnloadScene(bool sctiveScene = false);
	static void DoWhenSceneReady();
	static void CreateWorld(const std::string& name);
	static void CreateScene(const std::string& name, SDL_Point worldSizeInTiles);

private:
	static void InternLoad(const std::string& sceneName);
	static void InternUnload();
};