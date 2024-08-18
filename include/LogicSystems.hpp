#pragma once

#include <flecs.h>

#include "Components.hpp"

void SystemPlayerMovementInput(flecs::entity entity, const ComponentPosition &pos,
		const ComponentMovement &mov, const ComponentPlayerMovement&);

void SystemCameraUpdate(flecs::entity entity, const ComponentCamera&, const ComponentPosition& pos);
