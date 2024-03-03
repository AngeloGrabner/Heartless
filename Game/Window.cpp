#include "Window.h"
#include <iostream>

SDL_Window* Window::sWin = nullptr;

void Window::Init(const std::string& name, SDL_Rect area, Uint32 flags)
{
	sWin = SDL_CreateWindow(name.c_str(), area.x, area.y, area.w, area.h, flags);
	if (!sWin)
	{
		std::cout << "SDL error: " << SDL_GetError();
	}
}

void Window::Free()
{
	SDL_DestroyWindow(sWin);
}

void Window::SetSize(SDL_Point size)
{
	if (size.x <= 1)
		size.x = 1;
	if (size.y <= 1)
		size.y = 1;
	SDL_SetWindowSize(sWin, size.x, size.y);
}

void Window::SetDisplayMode(const SDL_DisplayMode& m)
{
	SDL_SetWindowDisplayMode(sWin, &m);
}

void Window::SetFullscreen(FullScreen flags)
{
	SDL_SetWindowFullscreen(sWin, flags);
}

void Window::SetIcon(const std::string& pathToIcon)
{
	SDL_Surface* sur = nullptr;
	sur = SDL_LoadBMP(pathToIcon.c_str());
	if (sur)
	{
		SDL_SetWindowIcon(sWin, sur);

		SDL_FreeSurface(sur);
	}
}

void Window::SetTitle(const std::string& titel)
{
	SDL_SetWindowTitle(sWin, titel.c_str());
}

SDL_DisplayMode Window::GetDispalyMode()
{
	SDL_DisplayMode m;
	SDL_GetWindowDisplayMode(sWin,&m);
	return m;
}

SDL_Window* Window::Get()
{
	return sWin;
}

SDL_Point Window::GetSize()
{
	SDL_Point p;
	SDL_GetWindowSize(sWin, &p.x, &p.y);
	return p;
}
