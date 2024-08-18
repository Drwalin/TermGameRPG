#pragma once

#include <flecs.h>

struct ComponentInput
{
	int character = 0;
};

void SystemInput(ComponentInput &comp);
