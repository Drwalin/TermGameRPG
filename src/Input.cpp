#include <ncurses.h>

#include "../include/Input.hpp"

void UpdateComponentInput(ComponentInput &comp)
{
	comp.character = getch();
	if (comp.character == ERR) {
		comp.character = 0;
	}
}
