#include "maze.h"
#include "line_renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
auto rng = std::default_random_engine(seed);
std::uniform_real_distribution<double> distribution(0.0f, 100.0f);

Maze::Maze(int nr, int nc, int w, int h, glm::vec3 col) : num_rows(nr), num_cols(nc), width(w), height(h), color(col) {
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

	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			for (int k = 0; k < 4; k++) {
				if (edges[i][j][k]) {
					if (distribution(rng) < 15.0f) {
						edges[i][j][k] = 0;
						if (k == 0 && i - 1 >= 0) {
							edges[i - 1][j][2] = 0;
						}
						if (k == 1 && j + 1 < num_cols) {
							edges[i][j + 1][3] = 0;
						}
						if (k == 2 && i + 1 < num_rows) {
							edges[i + 1][j][0] = 0;
						}
						if (k == 3 && j - 1 >= 0) {
							edges[i][j - 1][1] = 0;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < num_rows; i++) {
		edges[i][0][3] = 1;
		edges[i][num_cols - 1][1] = 1;
	}
	for (int j = 0; j < num_cols; j++) {
		edges[0][j][0] = 1;
		edges[num_rows - 1][j][2] = 1;
	}
}

void Maze::initRenderData() {
}

void Maze::Render(LineRenderer *line_renderer, glm::vec2 light_source, bool light) {
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			float distance = std::pow((light_source.x - i) * (light_source.x - i) + (light_source.y - j) * (light_source.y - j), 0.5);
			float intensity;
			if (distance == 0) {
				intensity = 1;
			} else {
				intensity = 1 * pow(2, -distance);
			}
			if (light) {
				intensity = 1;
			}
			if (edges[i][j][0]) {
				line_renderer->DrawLine(glm::vec2((j + 0) * width, (i + 0) * height), glm::vec2((j + 1) * width, (i + 0) * height), this->color, intensity);
			}
			if (edges[i][j][1]) {
				line_renderer->DrawLine(glm::vec2((j + 1) * width, (i + 0) * height), glm::vec2((j + 1) * width, (i + 1) * height), this->color, intensity);
			}
			if (edges[i][j][2]) {
				line_renderer->DrawLine(glm::vec2((j + 1) * width, (i + 1) * height), glm::vec2((j + 0) * width, (i + 1) * height), this->color, intensity);
			}
			if (edges[i][j][3]) {
				line_renderer->DrawLine(glm::vec2((j + 0) * width, (i + 1) * height), glm::vec2((j + 0) * width, (i + 0) * height), this->color, intensity);
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
		if (val == 0 && (x - 1) >= 0 && !vis[x - 1][y]) {
			edges[x][y][0] = 0;
			edges[x - 1][y][2] = 0;
			dfs(vis, x - 1, y);
		}
		//1
		if (val == 1 && (y + 1) < num_cols && !vis[x][y + 1]) {
			edges[x][y][1] = 0;
			edges[x][y + 1][3] = 0;
			dfs(vis, x, y + 1);
		}
		//2
		if (val == 2 && (x + 1) < num_rows && !vis[x + 1][y]) {
			edges[x][y][2] = 0;
			edges[x + 1][y][0] = 0;
			dfs(vis, x + 1, y);
		}
		//3
		if (val == 3 && (y - 1) >= 0 && !vis[x][y - 1]) {
			edges[x][y][3] = 0;
			edges[x][y - 1][1] = 0;
			dfs(vis, x, y - 1);
		}
	}
}
