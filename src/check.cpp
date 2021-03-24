#include <iostream>
#include "maze.h"

int main() {
	Maze maze(3, 3);

	for (auto x : maze.valid_edges) {
		std::cout << x.first.first << " " << x.first.second << " " << x.second.first << " " << x.second.second << " " << std::endl;
	}

	return 0;
}
