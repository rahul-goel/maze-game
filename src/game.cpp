#include "game.h"
#include "character.h"
#include <iostream>

float LEN = 20;
float CELL_SIZE = 50;

bool LIGHT = true;

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
	character = new Character(glm::vec2(0, 0), glm::vec2(CELL_SIZE, CELL_SIZE));

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
		// check whether key was not pressed before and is pressed now
		// find the cell of the character
		// move if not bounded by box
		{
			if (this->PrevKeys[GLFW_KEY_A] == false && this->Keys[GLFW_KEY_A] == true) {
				if (character->Position.y > 0) {
					int x = character->Position.x;
					int y = character->Position.y;
					if (maze->edges[x][y][3] == 0)  {
						character->Position.y -= 1;
					}
				}
			}
			if (this->PrevKeys[GLFW_KEY_D] == false && this->Keys[GLFW_KEY_D] == true) {
				if (character->Position.y < LEN - 1) {
					int x = character->Position.x;
					int y = character->Position.y;
					if (maze->edges[x][y][1] == 0)  {
						character->Position.y += 1;
					}
				}
			}
			if (this->PrevKeys[GLFW_KEY_S] == false && this->Keys[GLFW_KEY_S] == true) {
				if (character->Position.x < LEN - 1) {
					int x = character->Position.x;
					int y = character->Position.y;
					if (maze->edges[x][y][2] == 0)  {
						character->Position.x += 1;
					}
				}
			}
			if (this->PrevKeys[GLFW_KEY_W] == false && this->Keys[GLFW_KEY_W] == true) {
				if (character->Position.x > 0) {
					int x = character->Position.x;
					int y = character->Position.y;
					if (maze->edges[x][y][0] == 0)  {
						character->Position.x -= 1;
					}
				}
			}
		}
		if (this->PrevKeys[GLFW_KEY_L] == false && this->Keys[GLFW_KEY_L] == true) {
			LIGHT = !LIGHT;
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
	maze->Render(line_renderer, character->Position, LIGHT);
}
