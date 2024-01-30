#pragma once
#include <SDL.h>
#include <iostream>
#include "Utility.h"
#include "Texture.h"
#include "Color.h"

class Renderer
{
	static SDL_Renderer* sRen;
	static SDL_Rect sViewPort;
	static SDL_FPoint sScale;
	static SDL_FPoint sOffset;
	static bool sScreenToWorld;
public:
	enum VSync
	{
		VSYNC_DISABLE,
		VSYNC_ENABLE
	};
	static bool Init(SDL_Window* window);
	static void Free();
	static SDL_Renderer* Get();
	static void SetViewport(int x, int y, int w = -1, int h = -1);
	static SDL_Rect GetViewport();

	static void DrawTexture(Texture texture, const SDL_FRect& pos);
	static void DrawTexture(Texture texture, const SDL_FRect& pos,
		double angle, const SDL_FPoint& center = { FLT_MAX,FLT_MAX },
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	static void DrawTexture(Texture texture, const SDL_Rect& pos);
	static void DrawTexture(Texture texture, const SDL_Rect& pos,
		double angle, const SDL_Point& center = { INT_MAX,INT_MAX },
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	static void Clear();
	static void SetBlendMode(SDL_BlendMode mode);
	static SDL_BlendMode GetBlendMode();
	static void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	static void SetColor(Color c);
	static Color GetColor();
	static void UpdateScreen();
	static void SetTarget(SDL_Texture* texture = nullptr);
	static void SetVSync(VSync v);
	static void SetZoom(SDL_FPoint zoom);
	//static SDL_FPoint GetZoom(); // use Camera class for that 
	static void SetOffset(SDL_FPoint offset);
	//static SDL_FPoint GetOffset(); // use Caerma class for that
	static void SetScreenMode(bool drawWorld = false);
	static bool GetScreenMode();
	//simple draw functions 

	static void DrawRect(const SDL_Rect& rect);
	static void DrawRects(const SDL_Rect* rects, int count);
	static void DrawLine(SDL_Point p1, SDL_Point p2);
	static void DrawLines(const SDL_Point* points, int count);
	static void DrawPoint(SDL_Point p);
	static void DrawPoints(SDL_Point* points, int count);

	static void FillRect(const SDL_Rect& rect);
	static void FillRects(const SDL_Rect* rects, int count);

	static void DrawRect(const SDL_FRect& rect);
	static void DrawRects(const SDL_FRect* rects, int count);
	static void DrawLine(SDL_FPoint p1, SDL_FPoint p2);
	static void DrawLines(const SDL_FPoint* points, int count);
	static void DrawPoint(SDL_FPoint p);
	static void DrawPoints(const SDL_FPoint* points, int count);

	static void FillRect(const SDL_FRect& rect);
	static void FillRects(const SDL_FRect* rects, int count);
private: 
	static SDL_Rect Transform(SDL_Rect rect);
	static SDL_FRect Transform(SDL_FRect rect);
	static SDL_Point Transform(SDL_Point point);
	static SDL_FPoint Transform(SDL_FPoint point);

};