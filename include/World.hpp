#pragma once

#include <functional>
#include <map>
#include <unordered_set>
#include <cstdint>

#include <ncurses.h>

#include <flecs.h>

#include "EntityComponents.hpp"
#include "TickTimer.hpp"

class World
{
public:
	void Initialize();
	void InitializeWindows();
	void OneEpoch();
	void Render();
	
	template<typename Fun, typename ...Args>
	auto _System(const char *name, Fun &&func, std::function<void(Args...)> f)
	{
		return ecs.system<Args...>(name).each(std::move(func));
	}
	
	template<typename Fun>
	auto System(const char *name, Fun &&func)
	{
		return _System(name, std::move(func), std::function(func));
	}
	
public:
	flecs::world ecs;
	
	WINDOW *windowMap = nullptr;
	WINDOW *windowStats = nullptr;
	WINDOW *windowTopStats = nullptr;
	
	WINDOW *frameVertical = nullptr;
	WINDOW *frameHorizontal = nullptr;
	
	bool quit = false;
	
	std::map<PosType, std::map<PosType, std::unordered_set<EntityId>>> spatial;
	TickTimer timer;
};
