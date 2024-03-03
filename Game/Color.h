#pragma once
#include <stdint.h>

struct Color
{
	uint8_t r = 0, g = 0, b = 0, a = 255;
	Color() = default;
	constexpr Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255);

};	
static const Color
WHITE(255, 255, 255),
GREY(170,170,170),
DARK_GREY(85,85,85),
BLACK(0, 0, 0),

RED(255, 0, 0),
GREEN(0, 255, 0),
BLUE(0, 0, 255),
YELLOW(255, 255, 0),
CYAN(0, 255, 255),
MAGENTA(255, 0, 255),

DARK_RED(127, 0, 0),
DARK_GREEN(0, 127, 0),
DARK_BLUE(0, 0, 127),
DARK_YELLOW(127, 127, 0),
DARK_CYAN(0, 127, 127),
DARK_MAGENTA(127, 0, 127);

