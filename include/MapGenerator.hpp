#pragma once

#include <cstdio>

#include <vector>
#include <random>

class MapGenerator
{
public:
	struct generation_params {
		int r1_cutoff = 5;
		int r2_cutoff = 3;
		int reps = 3;
	};
	
public:
	int randpick(void);

	void initmap(void);

	void generation(void);

	void Generate(int w, int h, float fillprob,
				  std::vector<generation_params> generations);

public:
	inline const static bool TILE_FLOOR = false;
	inline const static bool TILE_WALL = true;

	std::vector<std::vector<bool>> grid, grid2;

	int fillprob = 40;
	int size_x = 64, size_y = 20;
	generation_params params;

	int generations;
};
