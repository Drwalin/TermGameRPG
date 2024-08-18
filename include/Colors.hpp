#pragma once

#include <cstdint>

enum Color : uint8_t
{
	BLACK = 0,
	RED = 1,
	GREEN = 2,
	BLUE = 4,
	YELLOW = RED | GREEN,
	CYAN = BLUE | GREEN,
	MAGENTA = RED | BLUE,
	WHITE = RED | GREEN | BLUE,
};

void InitColors();

int8_t GetColorPairID(Color backgroun, Color foreground);

void EnableColor(Color backgroun, Color foreground);
void DisableColor(Color backgroun, Color foreground);
