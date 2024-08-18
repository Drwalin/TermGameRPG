// C code downloaded from
// https://www.roguebasin.com/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels
// and modified

#include "../include/MapGenerator.hpp"

int MapGenerator::randpick(void)
{
	static std::random_device rd;
	static std::mt19937_64 mt(rd());
	if (mt() % 100 < fillprob)
		return TILE_WALL;
	else
		return TILE_FLOOR;
}

void MapGenerator::initmap(void)
{
	int xi, yi;
	grid.resize(size_y);

	for (yi = 0; yi < size_y; yi++) {
		grid[yi].resize(size_x);
	}

	for (yi = 1; yi < size_y - 1; yi++)
		for (xi = 1; xi < size_x - 1; xi++)
			grid[yi][xi] = randpick();

	for (yi = 0; yi < size_y; yi++)
		grid[yi][0] = grid[yi][size_x - 1] = TILE_WALL;
	for (xi = 0; xi < size_x; xi++)
		grid[0][xi] = grid[size_y - 1][xi] = TILE_WALL;

	grid2 = grid;
}

void MapGenerator::generation(void)
{
	int xi, yi, ii, jj;

	for (yi = 1; yi < size_y - 1; yi++) {
		for (xi = 1; xi < size_x - 1; xi++) {
			int adjcount_r1 = 0, adjcount_r2 = 0;

			for (ii = -1; ii <= 1; ii++)
				for (jj = -1; jj <= 1; jj++) {
					if (grid[yi + ii][xi + jj] != TILE_FLOOR)
						adjcount_r1++;
				}
			for (ii = yi - 2; ii <= yi + 2; ii++)
				for (jj = xi - 2; jj <= xi + 2; jj++) {
					if (abs(ii - yi) == 2 && abs(jj - xi) == 2)
						continue;
					if (ii < 0 || jj < 0 || ii >= size_y || jj >= size_x)
						continue;
					if (grid[ii][jj] != TILE_FLOOR)
						adjcount_r2++;
				}
			if (adjcount_r1 >= params.r1_cutoff ||
				adjcount_r2 <= params.r2_cutoff)
				grid2[yi][xi] = TILE_WALL;
			else
				grid2[yi][xi] = TILE_FLOOR;
		}
	}

	std::swap(grid, grid2);
}

void MapGenerator::Generate(
	int w, int h, float fillprob,
	std::vector<MapGenerator::generation_params> generations)
{
	int i, j;
	size_x = w;
	size_y = h;
	this->fillprob = fillprob * 100.0f + 0.5f;

	initmap();

	for (auto p : generations) {
		params = p;
		for (j = 0; j < params.reps; j++) {
			generation();
		}
	}
}
