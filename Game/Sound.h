#pragma once
#include <SDL_mixer.h>
#include "CSV.h"
#include <unordered_map>
#include <future>

class Sound
{
	static std::unordered_map<int, Mix_Chunk*> sChunkMap;
	static std::unordered_map<int, Mix_Music*> sMusicMap;
	static std::future<bool> sLoadFut;
public:
	// call WaitForLoad() after init
	static bool Init(const std::string& pathToCSV = "");

	// csv Format: id, type, path
	// call WaitForLoad() after load
	static void Load(const std::string& pathToCSV);

	static void WaitForLoad();

	static void Play(int soundId,int loops = 0, int chan = -1);
	static void Pause(int chan = -1);
	static void Resume(int chan = -1);
	static bool Playing(int chan);
	static int PlayingCount();
	
	static float GetVolume(int chan);
	static float GetChunkVolume(int chunkId);
	static void SetVolume(int chan, float normilizedVal);
	static void SetChunkVolume(int chunkId, float normilizedVal);

	static void PlayMusic(int musicId, int loops);
	static void PauseMusic();
	static void ResumeMusic();
	static void RewindMusic();
	static bool PlayingMusic();

	static float GetMasterVolum();
	static void SetMasterVolume(float normalizedVal);

	//unloads all audio files;
	static void Unload();

	//unload all audio files and frees the audio divce
	static void Free();
};

