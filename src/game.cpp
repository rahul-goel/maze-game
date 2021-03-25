#include "game.h"
#include "character.h"
#include <iostream>

float LEN = 20;
float CELL_SIZE = 50;

SpriteRenderer  *Renderer;
LineRenderer *line_renderer;
Maze *maze;
Character *character;

Game::Game(unsigned int width, unsigned int height) : Status(GAME_ACTIVE), Keys(), Width(width), Height(height) {
}

Game::~Game() {
}


void Game::Init() {
	// load shaders
	ResourceManager::LoadShader("../shaders/sprite.vs", "../shaders/sprite.frag", nullptr, "sprite");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
			static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// set render-specific controls
	Shader spriteShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(spriteShader);
	line_renderer = new LineRenderer(spriteShader);
	character = new Character(glm::vec2(0.0f, 0.0f), glm::vec2(CELL_SIZE, CELL_SIZE));

	maze = new Maze(LEN, LEN, CELL_SIZE, CELL_SIZE);
	maze->initRenderData();
}

void Game::Update(float dt) {
	for (int i = 0; i < 1024; i++) {
		this->PrevKeys[i] = this->Keys[i];
		this->Keys[i] = false;
	}
}

void Game::ProcessInput(float dt) {
	if (this->Status == GAME_ACTIVE) {
		if (this->PrevKeys[GLFW_KEY_A] == false && this->Keys[GLFW_KEY_A] == true) {
			if (character->Position.x > 0.0f) {
				character->Position.x -= CELL_SIZE;
			}
		}
		if (this->PrevKeys[GLFW_KEY_D] == false && this->Keys[GLFW_KEY_D] == true) {
			if (character->Position.x < 1000.0f - CELL_SIZE) {
				character->Position.x += CELL_SIZE;
			}
		}
		if (this->PrevKeys[GLFW_KEY_S] == false && this->Keys[GLFW_KEY_S] == true) {
			if (character->Position.y < 1000.0f - CELL_SIZE) {
				character->Position.y += CELL_SIZE;
			}
		}
		if (this->PrevKeys[GLFW_KEY_W] == false && this->Keys[GLFW_KEY_W] == true) {
			if (character->Position.y > 0.0f) {
				character->Position.y -= CELL_SIZE;
			}
		}
	}
}

void Game::Render() {
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
			static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	Texture2D tex;
	character->Render(Renderer);
	maze->Render(line_renderer);
}
