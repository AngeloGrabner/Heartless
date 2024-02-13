#pragma once
#include <SDL_mixer.h>
#include "CSV.h"

class Sound
{
	static bool Init();

	static void Play(Mix_Chunk* data, int chan = -1);
	static void Pause(int chan = -1);
	static void Resume(int chan = -1);

	static void PlayMusic(int soundId);
	static void PauseMusic();
	static void ResumeMusic();
	static void RewindMusic();

	static void MasterVolum();

	static void Free();
};

