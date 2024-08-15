#include <thread>

#include "../include/World.hpp"

int main() 
{
	initscr();
	cbreak();
	noecho();
	scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE);;
	
	World world;
	
	world.Initialize();
	
	while (world.quit == false) {
		world.OneEpoch();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
	endwin();
	return 0;
}
