#include "Texture.h"
void Texture::SetTint(Color c)
{
	SetTint(c.r, c.g, c.b, c.a);
}

Color Texture::GetColor() const
{
	Color c; 
	SDLCHECK(SDL_GetTextureColorMod(tex, &c.r, &c.g, &c.b));
	SDLCHECK(SDL_GetTextureAlphaMod(tex, &c.a));
	return c;
}

void Texture::SetTint(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDLCHECK(SDL_SetTextureColorMod(tex, r, g, b));
	SDLCHECK(SDL_SetTextureAlphaMod(tex, a));
}




