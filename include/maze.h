#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>
#include <set>
#include <algorithm>
#include <random>
#include <array>

class Maze {
	public:
		int num_rows;
		int num_cols;
		int num_cells;
		int num_edges;
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> valid_edges;

		Maze(int nr = 10, int nc = 10) {
			num_rows = nr;
			num_cols = nc;
			num_cells = num_rows * num_cols;
			num_edges = 2 * num_rows * num_cols + num_rows + num_cols;

			// store the edges
			std::vector<std::pair<int, int>> edges;
			for (int i = 0; i < num_rows; i++) {
				for (int j = 0; j < num_cols; j++) {
					if (j + 1 < num_cols) {
						edges.push_back(std::pair<int, int>(hash(i, j), hash(i, j + 1)));
					}
					if (i + 1 < num_rows) {
						edges.push_back(std::pair<int, int>(hash(i, j), hash(i + 1, j)));
					}
				}
			}

			//shuffle the edges
			auto rng = std::default_random_engine {};
			std::shuffle(std::begin(edges), std::end(edges), rng);

			// select some edges to form maze
			DSU dsu(num_cells);
			std::vector<std::pair<int, int>> drawn_edges;
			while (edges.size()) {
				int u = edges.back().first;
				int v = edges.back().second;
				if (dsu.find_set(u) == dsu.find_set(v)) {
					drawn_edges.push_back(edges.back());
				} else {
					dsu.union_set(u, v);
				}
				edges.pop_back();
			}

			for (std::pair<int, int> x : drawn_edges) {
				int u = x.first;
				int v = x.second;
				valid_edges.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(anti_hash(u),  anti_hash(v)));
			}
		}

		void get_segments(float left, float right, float top, float bottom, float solid = 1.0, float empty = 1.0) {
			std::vector<std::array<float, 5>> vec;
			for (float i = left; i < right; i += (right - left) / num_cols) {
				vec.push_back({i, i + (right - left) / num_cols});
			}
		}

	private:
		int hash(int x, int y){
			return x * num_cols + y;
		}

		std::pair<int, int> anti_hash(int val) {
			return std::pair<int, int>(val / num_cols, val % num_cols);
		}

		struct DSU {
			std::vector<int> parent, size;

			DSU(int n) {
				parent.resize(n);
				size.resize(n);

				for (int i = 0; i < n; i++) {
					parent[i] = i;
					size[i] = 1;
				}
			}

			int find_set(int v) {
				if (v == parent[v]) {
					return v;
				} else {
					return parent[v] = find_set(parent[v]);
				}
			}

			void union_set(int a, int b) {
				a = find_set(a);
				b = find_set(b);
				if(a != b){
					if(size[a] < size[b]) {
						std::swap(a, b);
					}
					parent[b] = a;
					size[a] += size[b];
				}
			}
		};
};

#endif
