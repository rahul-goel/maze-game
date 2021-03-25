#include "maze.h"
#include "line_renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <random>
#include <chrono>


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
auto rng = std::default_random_engine(seed);

Maze::Maze(int nr, int nc, int w, int h) : num_rows(nr), num_cols(nc), width(w), height(h) {
	edges.resize(nr, std::vector<std::array<int, 4>>(nc));
	num_cells = num_rows * num_cols;
	num_edges = 2 * num_rows * num_cols + num_rows + num_cols;

	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			for (int k = 0; k < 4; k++) {
				edges[i][j][k] = 1;
			}
		}
	}

	std::vector<std::vector<bool>> vis(nr, std::vector<bool>(nc));
	dfs(vis, 0, 0);
}

void Maze::initRenderData() {
}

void Maze::Render(LineRenderer *line_renderer) {
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			if (edges[i][j][0]) {
				line_renderer->DrawLine(glm::vec2((i + 0) * width, (j + 0) * height), glm::vec2((i + 1) * width, (j + 0) * height), glm::vec3(1.0f, 0.0f, 0.0f));
			}
			if (edges[i][j][1]) {
				line_renderer->DrawLine(glm::vec2((i + 1) * width, (j + 0) * height), glm::vec2((i + 1) * width, (j + 1) * height), glm::vec3(1.0f, 0.0f, 0.0f));
			}
			if (edges[i][j][2]) {
				line_renderer->DrawLine(glm::vec2((i + 1) * width, (j + 1) * height), glm::vec2((i + 0) * width, (j + 1) * height), glm::vec3(1.0f, 0.0f, 0.0f));
			}
			if (edges[i][j][3]) {
				line_renderer->DrawLine(glm::vec2((i + 0) * width, (j + 1) * height), glm::vec2((i + 0) * width, (j + 0) * height), glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}
	}
}

void Maze::dfs(std::vector<std::vector<bool>> &vis, int x, int y) {
	vis[x][y] = true;
	std::vector<int> vec({0, 1, 2, 3});

	std::shuffle(std::begin(vec), std::end(vec), rng);

	for (int val : vec) {
		//0
		if (val == 0 && (y - 1) >= 0 && !vis[x][y - 1]) {
			edges[x][y][0] = 0;
			edges[x][y - 1][2] = 0;
			dfs(vis, x, y - 1);
		}
		//1
		if (val == 1 && (x + 1) < num_rows && !vis[x + 1][y]) {
			edges[x][y][1] = 0;
			edges[x + 1][y][3] = 0;
			dfs(vis, x + 1, y);
		}
		//2
		if (val == 2 && (y + 1) < num_cols && !vis[x][y + 1]) {
			edges[x][y][2] = 0;
			edges[x][y + 1][0] = 0;
			dfs(vis, x, y + 1);
		}
		//3
		if (val == 3 && (x - 1) >= 0 && !vis[x - 1][y]) {
			edges[x][y][3] = 0;
			edges[x - 1][y][1] = 0;
			dfs(vis, x - 1, y);
		}
	}
}
