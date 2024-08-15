#include <ncurses.h>

#include "../include/Input.hpp"
#include "../include/World.hpp"

void World::Initialize()
{
	timer.Start();
	InitializeWindows();
	
	// init input systems
	ecs.entity().add<ComponentInput>();
	System("Input", UpdateComponentInput);
	
	// init logic systems
	
	
	
	
	
}

void World::InitializeWindows()
{
	windowMap = newwin(0, 0, 10, 20);
	windowStats = newwin(0, 21, 0, 0);
	windowTopStats = newwin(11, 0, 0, 20);
	
	frameVertical = newwin(0, 1, 0, 20);
	frameHorizontal = newwin(1, 0, 10, 21);
	
	int x, y;
	getmaxyx(frameVertical, y, x);
	for (int i=0; i<y; ++i)
		mvwaddch(frameVertical, i, 0, '|');
		
	getmaxyx(frameHorizontal, y, x);
	for (int i=0; i<x; ++i)
		mvwaddch(frameHorizontal, 0, i, '-');
}

void World::OneEpoch()
{
	timer.Update();
	
	ecs.progress();
	
	Render();
}

void World::Render()
{
	wrefresh(windowMap);
	wrefresh(windowStats);
	wrefresh(windowTopStats);
	wrefresh(frameVertical);
	wrefresh(frameHorizontal);
}
