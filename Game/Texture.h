#pragma once
#include <string>
#include <SDL.h>
#include "Color.h"
#include "Utility.h"
//warper 
struct Texture
{
	SDL_Texture* tex = nullptr;
	SDL_Rect rect = { 0,0,0,0 };
	
	Texture(SDL_Texture* texture, SDL_Rect Rect)
	{
		tex = texture;
		rect = Rect;
	}
	Texture() = default;
	void SetTint(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void SetTint(Color c);
	Color GetColor() const;
};