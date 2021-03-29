#include "game.h"
#include "character.h"
#include "object.h"
#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <ctime>
#include <chrono>
#include <thread>

// time
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

// colors
glm::vec3 RED(1.0f, 0.0f, 0.0f);
glm::vec3 GREEN(0.0f, 1.0f, 0.0f);
glm::vec3 BLUE(0.0f, 0.0f, 1.0f);
glm::vec3 YELLOW(1.0f, 1.0f, 0.0f);
glm::vec3 ORANGE(1.0f, 0.6f, 0.2f);

float LEN = 20;
float CELL_SIZE = 50;
bool LIGHT = true;
unsigned int SCORE = 0;
Time::time_point time_track;
unsigned int COUNTDOWN = 30;

SpriteRenderer  *Renderer;
LineRenderer *line_renderer;
CircleRenderer *circle_renderer;

Maze *maze;
Character *player;
Character *impostor;
Time::time_point impostor_move_time;
int impostor_time_delta = 300; //ms

Object *imp_vap_but;
Object *second_button;
std::vector<Object> obstacles;
std::vector<Object> powerups;

void every_second() {
	auto now = Time::now();
	auto diff = Time::now() - time_track;
	auto diff_ms = std::chrono::duration_cast<ms>(diff);
	if (diff_ms.count() < 1000) {
		return;
	}
	time_track = Time::now();
	COUNTDOWN -= 1;
	SCORE += 1;
}

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
	auto now = Time::now();
	auto diff = Time::now() - impostor_move_time;
	auto diff_ms = std::chrono::duration_cast<ms>(diff);
	if (diff_ms.count() < impostor_time_delta || impostor->Position == player->Position) {
		return impostor->Position;
	}

	impostor_move_time = Time::now();
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
	// random seed
	srand(time(nullptr));
	time_track = Time::now();

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
	impostor_move_time = Time::now();

	glm::vec2 imp_vap_but_pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
	glm::vec2 imp_vap_but_size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
	imp_vap_but = new Object(imp_vap_but_pos, imp_vap_but_size, BLUE);

	glm::vec2 second_button_pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
	glm::vec2 second_button_size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
	second_button = new Object(second_button_pos, second_button_size, GREEN);

	circle_renderer = new CircleRenderer(spriteShader);

	maze = new Maze(LEN, LEN, CELL_SIZE, CELL_SIZE);
	maze->initRenderData();
}

bool Game::Update(float dt) {
	every_second();

	for (int i = 0; i < 1024; i++) {
		this->PrevKeys[i] = this->Keys[i];
		this->Keys[i] = false;
	}
	impostor->Position = impostor_move();

	if (second_button->exist && second_button->Position == player->Position) {
		second_button->exist = false;
		for (int i = 0; i < 2; i++) {
			glm::vec2 pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
			glm::vec2 size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
			obstacles.push_back(Object(pos, size, YELLOW));
		}
		for (int i = 0; i < 2; i++) {
			glm::vec2 pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
			glm::vec2 size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
			powerups.push_back(Object(pos, size, ORANGE));
		}
	}

	for (auto &obs : obstacles) {
		if (obs.exist && obs.Position == player->Position) {
			SCORE -= 10;
			obs.exist = false;
		}
	}

	for (auto &po : powerups) {
		if (po.exist && po.Position == player->Position) {
			SCORE += 10;
			po.exist = false;
		}
	}

	if (imp_vap_but->exist && imp_vap_but->Position == player->Position) {
		imp_vap_but->exist = false;
		impostor->exist = false;
		SCORE += 20;
	}

	if (impostor->exist && impostor->Position == player->Position) {
		return true;
	}
	if (player->Position == glm::vec2(LEN - 1, LEN - 1)) {
		return true;
	}
	if (COUNTDOWN == 0) {
		return true;
	}
	return false;
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
	maze->Render(line_renderer, player->Position, LIGHT);

	if (impostor->exist) {
		impostor->Render(Renderer, circle_renderer, intensity_calc(impostor->Position));
	}
	if (imp_vap_but->exist) {
		imp_vap_but->Render(Renderer, circle_renderer);
	}
	if (second_button->exist) {
		second_button->Render(Renderer, circle_renderer);
	}

	for (auto &obs : obstacles) {
		if (obs.exist) {
			obs.Render(Renderer, circle_renderer);
		}
	}
	for (auto &po : powerups) {
		if (po.exist) {
			po.Render(Renderer, circle_renderer);
		}
	}

	std::cout << "SCORE: " << SCORE << std::endl;
	std::cout << "TIME: " << COUNTDOWN << std::endl;
}
