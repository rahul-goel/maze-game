#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>
#include <set>
#include <algorithm>
#include <random>
#include <array>

#include "line_renderer.h"

class Maze {
	public:
		const int num_rows;
		const int num_cols;
		const int width;
		const int height;
		int num_cells;
		int num_edges;
		std::vector<std::vector<std::array<int, 4>>> edges;
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> valid_edges;

		Maze(int nr = 10, int nc = 10, int w = 10, int h = 10);
		void initRenderData();
		void Render(LineRenderer *line_renderer);
	private:
		void dfs(std::vector<std::vector<bool>> &vis, int x, int y);
};

#endif
