#pragma once

#include <functional>
#include <map>
#include <unordered_set>
#include <cstdint>

#include <ncurses.h>

#include <flecs.h>

#include "Components.hpp"
#include "TickTimer.hpp"

class World;

struct WorldPtr {
	World *world;
};

class World
{
public:
	void Initialize();
	void InitializeWindows();
	void InitializeLogicSystems();
	void SpawnWorld();
	void OneEpoch();
	void Render();

	template <typename Fun, typename... Args>
	auto _System(const char *name, Fun &&func,
				 std::function<void(flecs::entity, Args...)> f)
	{
		return ecs.system<Args...>(name).each(std::move(func));
	}

	template <typename Fun, typename... Args>
	auto _System(const char *name, Fun &&func, std::function<void(Args...)> f)
	{
		return ecs.system<Args...>(name).each(std::move(func));
	}

	template <typename Fun> auto System(const char *name, Fun &&func)
	{
		return _System(name, std::move(func), std::function(func));
	}

public:
	EntityId GetBlockingEntityId(ComponentCollision &col, PosType x, PosType y);
	std::unordered_set<EntityId> *GetEntitiesIn(PosType x, PosType y);

	void ForceMoveTo(EntityId entityId, int x, int y);

public:
	flecs::world ecs;

	PosType cameraX = 0, cameraY = 0;

	WINDOW *windowMap = nullptr;
	WINDOW *windowStats = nullptr;
	WINDOW *windowTopStats = nullptr;

	WINDOW *frameVertical = nullptr;
	WINDOW *frameHorizontal = nullptr;

	bool quit = false;

	std::map<PosType, std::map<PosType, std::unordered_set<EntityId>>> spatial;
	TickTimer timer;
};
