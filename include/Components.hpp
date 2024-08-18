#pragma once

#include <cstdint>

#include "Colors.hpp"

using PosType = int32_t;
using EntityId = uint64_t;

struct ComponentPosition
{
	PosType x, y;
};

/*
 * 0 - no blocking collision (background)
 * 1 - has collision but can be many in one place (pickable)
 * 2 - has collision but can be only one of this type in one place (characters)
 * 255 - fully blocking collision
 */
struct ComponentCollision
{
	int32_t collisionMode;
};

struct ComponentMovement
{
	int64_t movementCooldown;
	int64_t lastMovementTimestamp;
};

struct ComponentRender
{
	int character = '#';
	Color backgroundColor = BLACK;
	Color foregroundColor = WHITE;
	
	int64_t lastDisplayedTimestamp = 0;
};

struct ComponentPlayerMovement
{
	int v;
};

struct ComponentAIRandomMovement
{
	int v;
};

struct ComponentHealth
{
	int maxHp;
	int hp;
};

struct ComponentCamera
{
	bool center = true;
};
