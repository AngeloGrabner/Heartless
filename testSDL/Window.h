#pragma once
#include <SDL.h>
#include <string>

//todo showWindow setTitle (showCursor)

class Window
{
	enum FullScreen
	{
		FULLSCREEN = SDL_WINDOW_FULLSCREEN,
		WINDOWED_FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP,
		WINDOW = 0
	};
	static SDL_Window* sWin;
public:
	static void Init(const std::string& name, SDL_Rect area, Uint32 flags = NULL);
	static void Free();

	static void SetSize(SDL_Point size);
	static void SetDisplayMode(const SDL_DisplayMode& m);
	static void SetFullscreen(FullScreen flags);

	static SDL_DisplayMode GetDispalyMode();
	static SDL_Window* Get();
	static SDL_Point GetSize();
};