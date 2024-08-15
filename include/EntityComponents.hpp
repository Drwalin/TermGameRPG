#pragma once

using PosType = int32_t;
using EntityId = uint64_t;

struct ComponentPosition
{
	PosType x, y;
};

struct ComopnentCurrentCharacter
{
	char character;
	int background;
	int foreground;
	
	int64_t lastDiplayedTimestamp;
	int64_t lastInViewportTimestamp;
};
