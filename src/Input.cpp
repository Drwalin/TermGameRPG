#include <ncurses.h>

#include "../include/Input.hpp"

void SystemInput(ComponentInput &comp)
{
	comp.character = getch();
	if (comp.character == ERR) {
		comp.character = 0;
	}
}
