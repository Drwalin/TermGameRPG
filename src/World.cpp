#include <random>

#include <ncurses.h>

#include "../include/Input.hpp"
#include "../include/MapGenerator.hpp"
#include "../include/LogicSystems.hpp"
#include "../include/World.hpp"

#define SYSTEM(FUNC) System(#FUNC, FUNC)

void World::Initialize()
{
	timer.Start();
	InitializeWindows();

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
	for (int i = 0; i < y; ++i)
		mvwaddch(frameVertical, i, 0, '|');

	getmaxyx(frameHorizontal, y, x);
	for (int i = 0; i < x; ++i)
		mvwaddch(frameHorizontal, 0, i, '-');
}

void World::InitializeLogicSystems()
{
	SYSTEM(SystemPlayerMovementInput);
	SYSTEM(SystemCameraUpdate);
}

void World::SpawnWorld()
{
	auto e = ecs.entity();
	e.set<ComponentPosition>({0, 0});
	e.set<ComponentCollision>({2});
	e.set<ComponentMovement>({});
	e.set<ComponentPlayerMovement>({});
	e.set<ComponentRender>({'X', RED, GREEN});
	e.set<ComponentCamera>({});
	ForceMoveTo(e.id(), 0, 0);

	// generate map
	MapGenerator map;
	int size = 256;
	map.Generate(size, size, 0.4, {{5, 2, 13}, {6, 1, 5}});

	for (int _x = 1; _x < size; ++_x) {
		for (int _y = 1; _y < size; ++_y) {
			int x = _x - size / 2;
			int y = _y - size / 2;
			if (map.grid[_y][_x] && (x * x + y * y * 2) < size * size / 5) {
				auto e = ecs.entity()
							 .set<ComponentPosition>({x, y})
							 .set<ComponentRender>({'#', BLACK, WHITE, false})
							 .set<ComponentCollision>({255});
				ForceMoveTo(e.id(), x, y);
			}
		}
	}

	// random grass
	std::random_device rd;
	std::mt19937_64 mt(rd());
	std::uniform_int_distribution posDist(-1000, 1000);
	static const char chars[] = ",.;:'\"`";
	std::uniform_int_distribution charDist(0, 6);
	static const Color colors[] = {GREEN, YELLOW, RED,	GREEN,
								   GREEN, GREEN,  GREEN};
	for (int i = 0; i < 100000; ++i) {
		int x, y;
		do {
			x = posDist(mt);
			y = posDist(mt);
		} while (spatial[x][y].size() != 0);
		char c = chars[charDist(mt)];
		Color color = colors[charDist(mt)];
		auto e =
			ecs.entity().set<ComponentPosition>({x, y}).set<ComponentRender>(
				{c, BLACK, color, true});
		ForceMoveTo(e.id(), x, y);
	}
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

	auto endx = spatial.lower_bound(cameraX + 2 + w / 2);
	for (auto itx = spatial.lower_bound(cameraX - 1 - w / 2); itx != endx;
		 ++itx) {
		auto endy = itx->second.lower_bound(cameraY + 2 + h / 2);
		for (auto ity = itx->second.lower_bound(cameraY - 1 - h / 2);
			 ity != endy; ++ity) {
			const auto &entities = ity->second;
			if (entities.size() == 0) {
				continue;
			}
			EntityId newest = 0;
			EntityId oldest = 0;
			ComponentRender newRend, oldRend;

			for (auto id : entities) {
				auto prend = flecs::entity(ecs, id).get<ComponentRender>();
				if (prend) {
					auto rend = *prend;
					if (oldest == 0) {
						newRend = oldRend = rend;
						newest = oldest = id;
					} else if (oldRend.lastDisplayedTimestamp >=
							   rend.lastDisplayedTimestamp) {
						oldRend = rend;
						oldest = id;
					} else if (newRend.lastDisplayedTimestamp <=
							   rend.lastDisplayedTimestamp) {
						newRend = rend;
						newest = id;
					}
				}
			}

			if (newest == 0) {
				continue;
			}

			if (newest != oldest) {
				if (newRend.lastDisplayedTimestamp + 666 < timer.currentTick) {
					newRend = oldRend;
					newest = oldest;

					newRend.lastDisplayedTimestamp = timer.currentTick;
					flecs::entity(ecs, newest).set(newRend);
				}
			} else if (newRend.lastDisplayedTimestamp != 50) {
				newRend.lastDisplayedTimestamp = 50;
				flecs::entity(ecs, newest).set(newRend);
			}

			EnableColor(windowMap, newRend.backgroundColor,
						newRend.foregroundColor);
			mvwaddch(windowMap, ity->first - (cameraY - h / 2),
					 itx->first - (cameraX + -w / 2), newRend.character);
			DisableColor(windowMap, newRend.backgroundColor,
						 newRend.foregroundColor);
		}
	}
	wprintw(windowTopStats, " %lli fps", c * 1000ll / (timer.currentTick + 1));

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

EntityId World::GetBlockingEntityId(ComponentCollision &col, PosType x,
									PosType y)
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
	if (auto *r = entity.get_mut<ComponentRender>()) {
		if (r->lastDisplayedTimestamp == 50) {
			r->lastDisplayedTimestamp = timer.currentTick;
		}
	}
}
