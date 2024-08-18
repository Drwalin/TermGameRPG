#include <ncurses.h>

#include "../include/Colors.hpp"

static int ToNCursesColor(Color color) {
	switch(color) {
		case BLACK: return COLOR_BLACK;
		case RED: return COLOR_RED;
		case GREEN: return COLOR_GREEN;
		case YELLOW: return COLOR_YELLOW;
		case BLUE: return COLOR_BLUE;
		case MAGENTA: return COLOR_MAGENTA;
		case CYAN: return COLOR_CYAN;
		case WHITE: return COLOR_WHITE;
	}
	return 0;
}

void InitColors()
{
	start_color();
	for (int b=0; b<8; ++b) {
		for (int f=0; f<8; ++f) {
			int id = GetColorPairID((Color)b, (Color)f);
			init_pair(id, ToNCursesColor((Color)f), ToNCursesColor((Color)b));
		}
	}
}

int8_t GetColorPairID(Color background, Color foreground)
{
	return (background | (foreground << 3)) + 1;
}

void EnableColor(Color background, Color foreground)
{
	attron(COLOR_PAIR(GetColorPairID(background, foreground)));
}

void DisableColor(Color background, Color foreground)
{
	attroff(COLOR_PAIR(GetColorPairID(background, foreground)));
}
