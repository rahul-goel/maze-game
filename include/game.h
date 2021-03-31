#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "maze.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "line_renderer.h"
#include "circle_renderer.h"
#include "character.h"
#include "text_renderer.h"
#include "object.h"

// numbers to represent the current state of the game
enum GameStatus {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_QUIT
};

class Game {
	public:
		// game properties currently
		GameStatus Status;
		bool Keys[1024];
		bool PrevKeys[1024];
		unsigned int Width, Height;

		//pointers

		SpriteRenderer  *Renderer = nullptr;
		LineRenderer *line_renderer = nullptr;
		CircleRenderer *circle_renderer = nullptr;
		TextRenderer *Text = nullptr;
		Maze *maze = nullptr;
		Character *player = nullptr;
		Character *impostor = nullptr;
		Object *imp_vap_but = nullptr;
		Object *second_button = nullptr;
		std::vector<Object> obstacles;
		std::vector<Object> powerups;

		// constructor/destructor
		Game(unsigned int width, unsigned int height);
		~Game();

		// to start the game
		void Init();

		// game loop
		void ProcessInput(float dt);
		bool Update(float dt);
		void Render();
	private:
		void end_text();
		glm::vec2 impostor_move();
		float intensity_calc(glm::vec2 pos);
};

#endif
