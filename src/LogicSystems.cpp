#include "../include/World.hpp"
#include "../include/Input.hpp"
#include "../include/LogicSystems.hpp"

void SystemPlayerMovementInput(flecs::entity entity,
							   const ComponentPosition &pos,
							   const ComponentMovement &mov,
							   const ComponentPlayerMovement &)
{
	auto *input = entity.world().get<ComponentInput>();
	auto *world = entity.world().get_mut<WorldPtr>()->world;
	int mvX = 0, mvY = 0;
	switch (input->character) {
	case 'h':
	case 'H':
		mvX = -1;
		break;
	case 'j':
	case 'J':
		mvY = +1;
		break;
	case 'k':
	case 'K':
		mvY = -1;
		break;
	case 'l':
	case 'L':
		mvX = +1;
		break;
	}
	if (mvX | mvY) {
		mvX += pos.x;
		mvY += pos.y;

		// TODO: check interactions first
		world->ForceMoveTo(entity.id(), mvX, mvY);
	}
}

void SystemCameraUpdate(flecs::entity entity, const ComponentCamera &,
						const ComponentPosition &pos)
{
	auto *world = entity.world().get_mut<WorldPtr>()->world;
	if (abs(world->cameraX - pos.x) > 10) {
		world->cameraX = pos.x;
	}

	if (abs(world->cameraY - pos.y) > 10) {
		world->cameraY = pos.y;
	}
	
	
	
	world->cameraX = pos.x;
	world->cameraY = pos.y;
}
