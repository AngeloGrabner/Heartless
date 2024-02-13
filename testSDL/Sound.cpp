#include "Sound.h"
#include <iostream>

bool Sound::Init()
{
    //Mix_Init(flags); 
    //AUDIO_U8 for 8 bit sound
    //2048 see remarks for details https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudioDevice
    if (0 > Mix_OpenAudio(44100,AUDIO_U8,2, 2048))
    {
        std::cout << "SDL error: " << Mix_GetError();
        return false;
    }
    
    return true;
}



void Sound::Free()
{
    Mix_CloseAudio();
    Mix_Quit();
}
