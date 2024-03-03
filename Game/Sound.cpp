#include "Sound.h"
#include "SDL_assert.h"
#include <iostream>
#include "Utility.h"

std::unordered_map<int, Mix_Chunk*> Sound::sChunkMap;
std::unordered_map<int, Mix_Music*> Sound::sMusicMap;

std::future<bool> Sound::sLoadFut;

bool Sound::Init(const std::string& pathToCSV)
{
    //Mix_Init(flags); 
    //AUDIO_U8 for 8 bit sound
    //2048 see remarks for details https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudioDevice
    if (0 > Mix_OpenAudio(44100,AUDIO_U8,2, 2048))
    {
        std::cout << "SDL error: " << Mix_GetError();
        SDL_assert(false);
        return false;
    }
    
    if (!pathToCSV.empty())
    {
        Load(pathToCSV);
    }

    return true;
}

void Sound::Load(const std::string& pathToCSV)
{
    sLoadFut = std::async([pathToCSV]() -> bool {
        CSV csv(pathToCSV);
        int rows = csv.GetRowCount();
        
        Mix_Music* mus = nullptr;
        Mix_Chunk* chu = nullptr;

        std::vector<std::string> row;
        int id = -1;
        char type = 0;
        std::string path; 

        for (int i = 0; i < rows; i++)
        {
            try
            {
                row = csv.GetRow(i);
                id = std::stoi(row[0]);
                type = row[1][0];
                path = row[2];

                if (path.empty())
                    continue;

                if (type == 'c')
                {
                    chu = Mix_LoadWAV(path.c_str());
                    if (chu)
                        sChunkMap[i] = chu;
                    else
                    {
                        LOG_PUSH(("couldn't load chunk audio file. path: " + path));
                        return false;
                    }
                }
                else if (type == 'm')
                {
                    mus = Mix_LoadMUS(path.c_str());
                    if (mus)
                        sMusicMap[i] = mus;
                    else
                    {
                        LOG_PUSH(("couldn't load music audio file. path: " + path));
                        return false;
                    }
                }
                else
                {
                    // ill formateted csv
                    return false;
                }
            }
            catch (std::exception& e)
            {
                LOG_PUSH(e.what());
            }
        }
        return true;
    });
}

void Sound::WaitForLoad()
{
    bool suc = sLoadFut.get();
    SDL_assert(suc);
}

void Sound::Play(int soundId,int loops, int chan )
{
    Mix_PlayChannel(chan, sChunkMap[soundId],loops);
}

void Sound::Pause(int chan)
{
    Mix_Pause(chan);
}

void Sound::Resume(int chan)
{
    Mix_Resume(chan);
}

bool Sound::Playing(int chan)
{
    return Mix_Playing(chan);
}

int Sound::PlayingCount()
{
    return Mix_Playing(-1);
}

float Sound::GetVolume(int chan)
{
    return Mix_Volume(chan,-1) / (float)MIX_MAX_VOLUME;
}

float Sound::GetChunkVolume(int chunkId)
{
    return Mix_VolumeChunk(sChunkMap[chunkId],-1) / (float)MIX_MAX_VOLUME;
}

void Sound::SetVolume(int chan, float normilizedVal)
{
    Mix_Volume(chan, (int)std::round(normilizedVal * MIX_MAX_VOLUME));
}

void Sound::SetChunkVolume(int chunkId, float normilizedVal)
{
    Mix_VolumeChunk(sChunkMap[chunkId], (int)std::round(normilizedVal * MIX_MAX_VOLUME));
}

void Sound::PlayMusic(int musicId, int loops)
{
    Mix_PlayMusic(sMusicMap[musicId], loops);
}

void Sound::PauseMusic()
{
    Mix_PauseMusic();
}

void Sound::ResumeMusic()
{
    Mix_ResumeMusic();
}

void Sound::RewindMusic()
{
    Mix_RewindMusic();
}

bool Sound::PlayingMusic()
{
    return Mix_PlayingMusic();
}

float Sound::GetMasterVolum()
{
    return Mix_MasterVolume(-1) / (float)MIX_MAX_VOLUME;
}

void Sound::SetMasterVolume(float normalizedVal)
{
    Mix_MasterVolume((int)std::round(normalizedVal * MIX_MAX_VOLUME));
}

void Sound::Unload()
{
    for (auto ptr : sChunkMap)
    {
        Mix_FreeChunk(ptr.second);
    }
    for (auto ptr : sMusicMap)
    {
        Mix_FreeMusic(ptr.second);
    }
}



void Sound::Free()
{
    Unload();

    Mix_CloseAudio();
    Mix_Quit();
}
