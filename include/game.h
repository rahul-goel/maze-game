#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "maze.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "line_renderer.h"

// numbers to represent the current state of the game
enum GameStatus {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game {
	public:
		// game properties currently
		GameStatus Status;
		bool Keys[1024];
		bool PrevKeys[1024];
		unsigned int Width, Height;

		// constructor/destructor
		Game(unsigned int width, unsigned int height);
		~Game();

		// to start the game
		void Init();

		// game loop
		void ProcessInput(float dt);
		void Update(float dt);
		void Render();
};

#endif
