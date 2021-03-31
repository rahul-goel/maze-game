#include "game.h"

#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

// time
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

// colors
glm::vec3 BACKGROUND((float) 40 / (float) 255, (float) 42 / (float) 255, (float) 54 / (float) 255);
glm::vec3 CURRENT((float) 68 / (float) 255, (float) 71 / (float) 255, (float) 90 / (float) 255);
glm::vec3 FOREGROUND((float) 248 / (float) 255, (float) 248 / (float) 255, (float) 242 / (float) 255);
glm::vec3 COMMENT((float) 98 / (float) 255, (float) 114 / (float) 255, (float) 164 / (float) 255);
glm::vec3 CYAN((float) 139 / (float) 255, (float) 233 / (float) 255, (float) 253 / (float) 255);
glm::vec3 GREEN((float) 80 / (float) 255, (float) 250 / (float) 255, (float) 123 / (float) 255);
glm::vec3 ORANGE((float) 255 / (float) 255, (float) 184 / (float) 255, (float) 108 / (float) 255);
glm::vec3 PINK((float) 255 / (float) 255, (float) 121 / (float) 255, (float) 198 / (float) 255);
glm::vec3 PURPLE((float) 189 / (float) 255, (float) 147 / (float) 255, (float) 249 / (float) 255);
glm::vec3 RED((float) 255 / (float) 255, (float) 85 / (float) 255, (float) 85 / (float) 255);
glm::vec3 YELLOW((float) 241 / (float) 255, (float) 250 / (float) 255, (float) 140 / (float) 255);

float LEN = 20;
float CELL_SIZE = 50;
bool LIGHT = true;
unsigned int SCORE = 0;
Time::time_point time_track;
unsigned int COUNTDOWN = 100;
unsigned int TASKS = 2;
int HEALTH = 5;

Time::time_point impostor_move_time;
int impostor_time_delta = 500; //ms

void Game::end_text() {
	this->Text->RenderText("GAME OVER.", (float) this->Width / 2, (float) this->Height / 2, 1.0f);
}

void every_second() {
	auto now = Time::now();
	auto diff = Time::now() - time_track;
	auto diff_ms = std::chrono::duration_cast<ms>(diff);
	if (diff_ms.count() < 1000) {
		return;
	}
	time_track = Time::now();
	COUNTDOWN -= 1;
	SCORE += (LIGHT ? 1 : 3);
}

float Game::intensity_calc(glm::vec2 pos) {
	glm::vec2 light_source = this->player->Position;
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

glm::vec2 Game::impostor_move() {
	auto now = Time::now();
	auto diff = Time::now() - impostor_move_time;
	auto diff_ms = std::chrono::duration_cast<ms>(diff);
	if (diff_ms.count() < impostor_time_delta || this->impostor->Position == this->player->Position) {
		return this->impostor->Position;
	}

	impostor_move_time = Time::now();
	std::queue<std::pair<int,int>> q;
	std::vector<std::vector<bool>> vis(LEN, std::vector<bool>(LEN, false));
	std::vector<std::vector<int>> dist(LEN, std::vector<int>(LEN, INT_MAX));
	int x = this->player->Position.x;
	int y = this->player->Position.y;
	q.push({x, y});
	vis[x][y] = true;
	dist[x][y] = 0;

	while (!q.empty()) {
		if (vis[this->impostor->Position.x][this->impostor->Position.y]) {
			break;
		}
		auto cur = q.front();
		q.pop();
		if (!this->maze->edges[cur.first][cur.second][0] && cur.first - 1 >= 0 && !vis[cur.first - 1][cur.second]) {
			vis[cur.first - 1][cur.second] = true;
			q.push({cur.first - 1, cur.second});
			dist[cur.first - 1][cur.second] = dist[cur.first][cur.second] + 1;
		}
		if (!this->maze->edges[cur.first][cur.second][1] && cur.second + 1 < LEN && !vis[cur.first][cur.second + 1]) {
			vis[cur.first][cur.second + 1] = true;
			q.push({cur.first, cur.second + 1});
			dist[cur.first][cur.second + 1] = dist[cur.first][cur.second] + 1;
		}
		if (!this->maze->edges[cur.first][cur.second][2] && cur.first + 1 < LEN && !vis[cur.first + 1][cur.second]) {
			vis[cur.first + 1][cur.second] = true;
			q.push({cur.first + 1, cur.second});
			dist[cur.first + 1][cur.second] = dist[cur.first][cur.second] + 1;
		}
		if (!this->maze->edges[cur.first][cur.second][3] && cur.second - 1 >= 0 && !vis[cur.first][cur.second - 1]) {
			vis[cur.first][cur.second - 1] = true;
			q.push({cur.first, cur.second - 1});
			dist[cur.first][cur.second - 1] = dist[cur.first][cur.second] + 1;
		}
	}

	int ix = this->impostor->Position.x;
	int iy = this->impostor->Position.y;
	if (ix - 1 >= 0 && dist[ix][iy] == dist[ix - 1][iy] + 1 && this->maze->edges[ix][iy][0] == 0) {
		return glm::vec2(ix - 1, iy);
	}
	if (iy + 1 < LEN && dist[ix][iy] == dist[ix][iy + 1] + 1 && this->maze->edges[ix][iy][1] == 0) {
		return glm::vec2(ix, iy + 1);
	}
	if (ix + 1 < LEN && dist[ix][iy] == dist[ix + 1][iy] + 1 && this->maze->edges[ix][iy][2] == 0) {
		return glm::vec2(ix + 1, iy);
	}
	if (iy - 1 >= 0 && dist[ix][iy] == dist[ix][iy - 1] + 1 && this->maze->edges[ix][iy][3] == 0) {
		return glm::vec2(ix, iy - 1);
	}

	//should not reach
	return glm::vec2(ix, iy);
}

Game::Game(unsigned int width, unsigned int height) : Status(GAME_ACTIVE), Keys(), Width(width), Height(height) {
}

Game::~Game() {
	delete this->player;
	delete this->impostor;
	delete this->imp_vap_but;
	delete this->Renderer;
	delete this->line_renderer;
	delete this->circle_renderer;
	delete this->Text;
	delete this->maze;
	delete this->second_button;
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

	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	//std::cout << "hello" << std::endl;
	//auto e = glGetError();
	//if (e != 0) {
	//	std::cout << e << std::endl;
	//	exit(1);
	//}

	// set render-specific controls
	Shader spriteShader = ResourceManager::GetShader("sprite");
	this->Renderer = new SpriteRenderer(spriteShader);
	this->line_renderer = new LineRenderer(spriteShader);
	this->player = new Character(glm::vec2(0, 0), glm::vec2(CELL_SIZE, CELL_SIZE), YELLOW, PURPLE);

	this->impostor = new Character(glm::vec2(LEN - 1, LEN - 1), glm::vec2(CELL_SIZE, CELL_SIZE), YELLOW, PINK);
	impostor_move_time = Time::now();

	glm::vec2 imp_vap_but_pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
	glm::vec2 imp_vap_but_size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
	this->imp_vap_but = new Object(imp_vap_but_pos, imp_vap_but_size, CYAN);

	glm::vec2 second_button_pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
	glm::vec2 second_button_size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
	this->second_button = new Object(second_button_pos, second_button_size, GREEN);

	this->circle_renderer = new CircleRenderer(spriteShader);

	this->Text = new TextRenderer(projection);
	this->Text->Load("../fonts/ocraext.TTF", 15);

	this->maze = new Maze(LEN, LEN, CELL_SIZE, CELL_SIZE);
	this->maze->initRenderData();
}

bool Game::Update(float dt) {
	every_second();

	for (int i = 0; i < 1024; i++) {
		this->PrevKeys[i] = this->Keys[i];
		this->Keys[i] = false;
	}
	this->impostor->Position = this->impostor_move();

	if (this->second_button->exist && this->second_button->Position == this->player->Position) {
		this->second_button->exist = false;
		TASKS--;
		for (int i = 0; i < 4; i++) {
			glm::vec2 pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
			glm::vec2 size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
			this->obstacles.push_back(Object(pos, size, COMMENT));
		}
		for (int i = 0; i < 4; i++) {
			glm::vec2 pos = glm::vec2(rand() % (int) LEN, rand() % (int) LEN);
			glm::vec2 size = glm::vec2(glm::vec2(CELL_SIZE, CELL_SIZE));
			this->powerups.push_back(Object(pos, size, YELLOW));
		}
	}

	for (auto &obs : this->obstacles) {
		if (obs.exist && obs.Position == this->player->Position) {
			SCORE -= 10;
			HEALTH -= 2;
			if (SCORE < 0) {
				SCORE = 0;
			}
			obs.exist = false;
		}
	}

	for (auto &po : this->powerups) {
		if (po.exist && po.Position == this->player->Position) {
			SCORE += 10;
			po.exist = false;
		}
	}

	if (this->imp_vap_but->exist && this->imp_vap_but->Position == this->player->Position) {
		this->imp_vap_but->exist = false;
		TASKS--;
		this->impostor->exist = false;
		SCORE += 20;
	}

	if (this->impostor->exist && this->impostor->Position == this->player->Position) {
		HEALTH = 0;
	}
	if (TASKS == 0 && this->player->Position == glm::vec2(LEN - 1, LEN - 1)) {
		this->Status = GAME_QUIT;
	}
	if (COUNTDOWN == 0 || HEALTH <= 0) {
		this->Status = GAME_QUIT;
	}

	if (this->Status == GAME_QUIT) {
		return true;
	} else {
		return false;
	}
}

void Game::ProcessInput(float dt) {
	if (this->Status == GAME_ACTIVE) {
		// check whether key was not pressed before and is pressed now
		// find the cell of the character
		// move if not bounded by box
		{
			if ((this->PrevKeys[GLFW_KEY_A] == false && this->Keys[GLFW_KEY_A] == true)
					|| (this->PrevKeys[GLFW_KEY_LEFT] == false && this->Keys[GLFW_KEY_LEFT] == true)) {
				if (this->player->Position.y > 0) {
					int x = this->player->Position.x;
					int y = this->player->Position.y;
					if (this->maze->edges[x][y][3] == 0)  {
						this->player->Position.y -= 1;
					}
				}
			}
			if ((this->PrevKeys[GLFW_KEY_D] == false && this->Keys[GLFW_KEY_D] == true)
					|| (this->PrevKeys[GLFW_KEY_RIGHT] == false && this->Keys[GLFW_KEY_RIGHT] == true)) {
				if (this->player->Position.y < LEN - 1) {
					int x = this->player->Position.x;
					int y = this->player->Position.y;
					if (this->maze->edges[x][y][1] == 0)  {
						this->player->Position.y += 1;
					}
				}
			}
			if ((this->PrevKeys[GLFW_KEY_S] == false && this->Keys[GLFW_KEY_S] == true)
					|| (this->PrevKeys[GLFW_KEY_DOWN] == false && this->Keys[GLFW_KEY_DOWN] == true)) {
				if (this->player->Position.x < LEN - 1) {
					int x = this->player->Position.x;
					int y = this->player->Position.y;
					if (this->maze->edges[x][y][2] == 0)  {
						this->player->Position.x += 1;
					}
				}
			}
			if ((this->PrevKeys[GLFW_KEY_W] == false && this->Keys[GLFW_KEY_W] == true)
					|| (this->PrevKeys[GLFW_KEY_UP] == false && this->Keys[GLFW_KEY_UP] == true)) {
				if (this->player->Position.x > 0) {
					int x = this->player->Position.x;
					int y = this->player->Position.y;
					if (this->maze->edges[x][y][0] == 0)  {
						this->player->Position.x -= 1;
					}
				}
			}
		}
		if (this->PrevKeys[GLFW_KEY_L] == false && this->Keys[GLFW_KEY_L] == true) {
			LIGHT = !LIGHT;
		}

		if (this->Keys[GLFW_KEY_Q] == true) {
			this->Status = GAME_QUIT;
		}
	}
}

void Game::Render() {
	Texture2D tex;
	// background
	// Renderer->DrawSprite(tex, glm::vec2(0,0), glm::vec2(this->Width, this->Height), 0, BACKGROUND, 1);
	this->player->Render(this->Renderer, this->circle_renderer);
	this->maze->Render(this->line_renderer, this->player->Position, LIGHT);


	if (this->impostor->exist) {
		this->impostor->Render(this->Renderer, this->circle_renderer, intensity_calc(this->impostor->Position));
	}
	if (this->imp_vap_but->exist) {
		this->imp_vap_but->Render(this->Renderer, this->circle_renderer, intensity_calc(this->imp_vap_but->Position));
	}
	if (this->second_button->exist) {
		this->second_button->Render(this->Renderer, this->circle_renderer, intensity_calc(this->second_button->Position));
	}

	for (auto &obs : this->obstacles) {
		if (obs.exist) {
			obs.Render(this->Renderer, this->circle_renderer, intensity_calc(obs.Position));
		}
	}
	for (auto &po : this->powerups) {
		if (po.exist) {
			po.Render(this->Renderer, this->circle_renderer, intensity_calc(po.Position));
		}
	}

	std::stringstream ss;
	ss << SCORE;
	this->Text->RenderText("Score:" + ss.str(), 5.0f, 5.0f + 12.0f * 0, 1.0f);
	ss.str("");
	ss << 2 - TASKS;
	this->Text->RenderText("Tasks:" + ss.str() + "/2", 5.0f, 5.0f + 12.0f * 1, 1.0f);
	ss.str("");
	ss << COUNTDOWN;
	this->Text->RenderText("Time:" + ss.str(), 5.0f, 5.0f + 12.0f * 2, 1.0f);
	ss.str("");
	ss << (LIGHT ? "ON" : "OFF");
	this->Text->RenderText("Light:" + ss.str(), 5.0f, 5.0f + 12.0f * 3, 1.0f);
	ss.str("");
	ss << HEALTH;
	this->Text->RenderText("Health:" + ss.str(), 5.0f, 5.0f + 12.0f * 4, 1.0f);

	if (this->Status == GAME_QUIT) {
		this->end_text();
	}
}
