#include <ncurses.h>

#include "../include/Input.hpp"
#include "../include/LogicSystems.hpp"
#include "../include/World.hpp"

#define SYSTEM(FUNC) System(#FUNC, FUNC)

void World::Initialize()
{
	timer.Start();
	InitializeWindows();
	
	ecs.component<WorldPtr>();
	ecs.component<ComponentInput>();
	ecs.component<ComponentPosition>();
	ecs.component<ComponentCollision>();
	ecs.component<ComponentMovement>();
	ecs.component<ComponentRender>();
	ecs.component<ComponentPlayerMovement>();
	ecs.component<ComponentAIRandomMovement>();
	ecs.component<ComponentHealth>();
	ecs.component<ComponentCamera>();
	
	ecs.set<WorldPtr>({this});
	ecs.set<ComponentInput>({});
	SYSTEM(SystemInput);
	
	SpawnWorld();
	InitializeLogicSystems();
	
	timer.Start();
	
}

void World::InitializeWindows()
{
	InitColors();
	windowMap = newwin(0, 0, 10, 20);
	windowStats = newwin(0, 21, 0, 0);
	windowTopStats = newwin(11, 0, 0, 20);
	
	frameVertical = newwin(0, 1, 0, 20);
	frameHorizontal = newwin(1, 0, 10, 21);
	leaveok(frameHorizontal, true);
	leaveok(frameVertical, true);
	
	int x, y;
	getmaxyx(frameVertical, y, x);
	for (int i=0; i<y; ++i)
		mvwaddch(frameVertical, i, 0, '|');
		
	getmaxyx(frameHorizontal, y, x);
	for (int i=0; i<x; ++i)
		mvwaddch(frameHorizontal, 0, i, '-');
}

void World::InitializeLogicSystems()
{
	SYSTEM(SystemPlayerMovementInput);
}

void World::SpawnWorld()
{
	auto e = ecs.entity();
		e.set<ComponentPosition>({0,0});
		e.set<ComponentCollision>({2});
		e.set<ComponentMovement>({});
		e.set<ComponentPlayerMovement>({});
		e.set<ComponentRender>({'X', RED, GREEN});
	ForceMoveTo(e.id(), 0, 0);
}

int c = 0;
void World::OneEpoch()
{
	++c;
	timer.Update();
	
	ecs.progress();
	
	Render();
}

void World::Render()
{
	werase(windowMap);
	werase(windowStats);
	werase(windowTopStats);
	
	int w, h;
	getmaxyx(windowMap, h, w);
	
	auto endx = spatial.lower_bound(cameraX+2+w/2);
	for (auto itx = spatial.lower_bound(cameraX-1-w/2); itx != endx; ++itx) {
		auto endy = itx->second.lower_bound(cameraX+2+h/2);
		for (auto ity = itx->second.lower_bound(cameraY-1-h/2); ity != endy; ++ity) {
			const auto &entities = ity->second;
			EntityId oldest = 0;
			ComponentRender newRend, oldRend;
			oldRend.lastDisplayedTimestamp = timer.currentTick+10000000l;
			for (auto id : entities) {
				ComponentRender rend = *flecs::entity(ecs, id).get<ComponentRender>();
				if (oldRend.lastDisplayedTimestamp >= rend.lastDisplayedTimestamp) {
					oldRend = rend;
					oldest = id;
				}
				if (newRend.lastDisplayedTimestamp <= rend.lastDisplayedTimestamp) {
					newRend = rend;
				}
			}
			if (entities.size()) {
				if (entities.size() > 1) {
					if (newRend.lastDisplayedTimestamp+500 < timer.currentTick) {
						newRend = oldRend;
						newRend.lastDisplayedTimestamp = timer.currentTick;
						flecs::entity(ecs, oldest).set<ComponentRender>(newRend);
					}
				}
				wprintw(windowTopStats, " %lli fps: %c, {%i,%i}", c*1000ll/(timer.currentTick+1), newRend.character, itx->first, ity->first);
				
				EnableColor(newRend.backgroundColor, newRend.foregroundColor);
				mvwaddch(windowMap, ity->first-(cameraY-h/2), itx->first-(cameraX+-w/2), newRend.character);
				DisableColor(newRend.backgroundColor, newRend.foregroundColor);
			}
		}
	}
	
	
	redrawwin(windowMap);
	redrawwin(windowStats);
	redrawwin(windowTopStats);
	wnoutrefresh(windowMap);
	wnoutrefresh(windowStats);
	wnoutrefresh(windowTopStats);
	
	redrawwin(frameVertical);
	redrawwin(frameHorizontal);
	wnoutrefresh(frameVertical);
	wnoutrefresh(frameHorizontal);
	
	doupdate();
}

EntityId World::GetBlockingEntityId(ComponentCollision &col, PosType x, PosType y)
{
	return 0;
}

std::unordered_set<EntityId> *World::GetEntitiesIn(PosType x, PosType y)
{
	auto it = spatial.find(x);
	if (it != spatial.end()) {
		auto it2 = it->second.find(y);
		if (it2 != it->second.end()) {
			return &it2->second;
		}
	}
	return nullptr;
}

void World::ForceMoveTo(EntityId entityId, int x, int y)
{
	flecs::entity entity(ecs, entityId);
	const ComponentPosition *p = entity.get<ComponentPosition>();
	if (p) {
		auto o = GetEntitiesIn(p->x, p->y);
		if (o) {
			o->erase(entityId);
		}
		spatial[x][y].insert(entityId);
	}
	entity.set<ComponentPosition>({x, y});
}
