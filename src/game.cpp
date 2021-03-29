#include "game.h"
#include "character.h"
#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <ctime>
#include <chrono>

float LEN = 20;
float CELL_SIZE = 50;

bool LIGHT = true;

SpriteRenderer  *Renderer;
LineRenderer *line_renderer;
CircleRenderer *circle_renderer;
Maze *maze;
Character *player;

Character *impostor;
time_t impostor_move_time = 0;
time_t impostor_time_delta = 1;

float intensity_calc(glm::vec2 pos) {
	glm::vec2 light_source = player->Position;
	float distance = std::pow((light_source.x - pos.x) * (light_source.x - pos.x) + (light_source.y - pos.y) * (light_source.y - pos.y), 0.5);
	float intensity;
	if (distance == 0) {
		intensity = 1;
	} else {
		intensity = 1 * pow(2, -distance);
	}
	if (LIGHT) {
		intensity = 1;
	}
	return intensity;
}

glm::vec2 impostor_move() {
	if (time(nullptr) < impostor_move_time + impostor_time_delta || impostor->Position == player->Position) {
		return impostor->Position;
	}

	impostor_move_time = time(nullptr);
	std::queue<std::pair<int,int>> q;
	std::vector<std::vector<bool>> vis(LEN, std::vector<bool>(LEN, false));
	std::vector<std::vector<int>> dist(LEN, std::vector<int>(LEN, INT_MAX));
	int x = player->Position.x;
	int y = player->Position.y;
	q.push({x, y});
	vis[x][y] = true;
	dist[x][y] = 0;

	while (!q.empty()) {
		if (vis[impostor->Position.x][impostor->Position.y]) {
			break;
		}
		auto cur = q.front();
		q.pop();
		if (!maze->edges[cur.first][cur.second][0] && cur.first - 1 >= 0 && !vis[cur.first - 1][cur.second]) {
			vis[cur.first - 1][cur.second] = true;
			q.push({cur.first - 1, cur.second});
			dist[cur.first - 1][cur.second] = dist[cur.first][cur.second] + 1;
		}
		if (!maze->edges[cur.first][cur.second][1] && cur.second + 1 < LEN && !vis[cur.first][cur.second + 1]) {
			vis[cur.first][cur.second + 1] = true;
			q.push({cur.first, cur.second + 1});
			dist[cur.first][cur.second + 1] = dist[cur.first][cur.second] + 1;
		}
		if (!maze->edges[cur.first][cur.second][2] && cur.first + 1 < LEN && !vis[cur.first + 1][cur.second]) {
			vis[cur.first + 1][cur.second] = true;
			q.push({cur.first + 1, cur.second});
			dist[cur.first + 1][cur.second] = dist[cur.first][cur.second] + 1;
		}
		if (!maze->edges[cur.first][cur.second][3] && cur.second - 1 >= 0 && !vis[cur.first][cur.second - 1]) {
			vis[cur.first][cur.second - 1] = true;
			q.push({cur.first, cur.second - 1});
			dist[cur.first][cur.second - 1] = dist[cur.first][cur.second] + 1;
		}
	}

	int ix = impostor->Position.x;
	int iy = impostor->Position.y;
	if (ix - 1 >= 0 && dist[ix][iy] == dist[ix - 1][iy] + 1 && maze->edges[ix][iy][0] == 0) {
		return glm::vec2(ix - 1, iy);
	}
	if (iy + 1 < LEN && dist[ix][iy] == dist[ix][iy + 1] + 1 && maze->edges[ix][iy][1] == 0) {
		return glm::vec2(ix, iy + 1);
	}
	if (ix + 1 < LEN && dist[ix][iy] == dist[ix + 1][iy] + 1 && maze->edges[ix][iy][2] == 0) {
		return glm::vec2(ix + 1, iy);
	}
	if (iy - 1 >= 0 && dist[ix][iy] == dist[ix][iy - 1] + 1 && maze->edges[ix][iy][3] == 0) {
		return glm::vec2(ix, iy - 1);
	}

	//should not reach
	return glm::vec2(ix, iy);
}

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
	player = new Character(glm::vec2(0, 0), glm::vec2(CELL_SIZE, CELL_SIZE), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	impostor = new Character(glm::vec2(LEN - 1, LEN - 1), glm::vec2(CELL_SIZE, CELL_SIZE), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	impostor_move_time = time(nullptr);

	circle_renderer = new CircleRenderer(spriteShader);

	maze = new Maze(LEN, LEN, CELL_SIZE, CELL_SIZE);
	maze->initRenderData();
}

void Game::Update(float dt) {
	for (int i = 0; i < 1024; i++) {
		this->PrevKeys[i] = this->Keys[i];
		this->Keys[i] = false;
	}
	impostor->Position = impostor_move();
}

void Game::ProcessInput(float dt) {
	if (this->Status == GAME_ACTIVE) {
		// check whether key was not pressed before and is pressed now
		// find the cell of the character
		// move if not bounded by box
		{
			if (this->PrevKeys[GLFW_KEY_A] == false && this->Keys[GLFW_KEY_A] == true) {
				if (player->Position.y > 0) {
					int x = player->Position.x;
					int y = player->Position.y;
					if (maze->edges[x][y][3] == 0)  {
						player->Position.y -= 1;
					}
				}
			}
			if (this->PrevKeys[GLFW_KEY_D] == false && this->Keys[GLFW_KEY_D] == true) {
				if (player->Position.y < LEN - 1) {
					int x = player->Position.x;
					int y = player->Position.y;
					if (maze->edges[x][y][1] == 0)  {
						player->Position.y += 1;
					}
				}
			}
			if (this->PrevKeys[GLFW_KEY_S] == false && this->Keys[GLFW_KEY_S] == true) {
				if (player->Position.x < LEN - 1) {
					int x = player->Position.x;
					int y = player->Position.y;
					if (maze->edges[x][y][2] == 0)  {
						player->Position.x += 1;
					}
				}
			}
			if (this->PrevKeys[GLFW_KEY_W] == false && this->Keys[GLFW_KEY_W] == true) {
				if (player->Position.x > 0) {
					int x = player->Position.x;
					int y = player->Position.y;
					if (maze->edges[x][y][0] == 0)  {
						player->Position.x -= 1;
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
	player->Render(Renderer, circle_renderer);
	impostor->Render(Renderer, circle_renderer, intensity_calc(impostor->Position));
	maze->Render(line_renderer, player->Position, LIGHT);
}
