#ifndef OBJECTS_H
#define OBJECTS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <math.h>
#include <iostream>

const int num = 12 * 3 * (3 + 3);

const float OBJSPEED = 2.5f;

enum Object_Movement {
	OZPLUS,
	OZMINUS,
	OXPLUS,
	OXMINUS,
	OYPLUS,
	OYMINUS
};

class HexagonalBipyramid {
public:
	const unsigned int size = 12 * 3 * (3 + 3);
	const unsigned int numPoints = 12 * 3;
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 positions[12 * 6];

	HexagonalBipyramid() {
		// total size of positions = 12 * 3 * 6 | triangle * num_points * num_coordinates
		float fac = 0.5;
		float pi = 3.14159265359;
		float r, g, b;

		for (int i = 0, j = 0; i < 6; i++) {
			// triangle of i, i + 1, top
			r = (float) rand() / (float) RAND_MAX;
			g = (float) rand() / (float) RAND_MAX;
			b = (float) rand() / (float) RAND_MAX;
			positions[j++] = glm::vec3(fac * std::cos(pi / 3 * i), fac * std::sin(pi / 3 * i), 0.0f);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(pi / 3 * ((i + 1) % 6)), fac * std::sin(pi / 3 * ((i + 1) % 6)), 0.0f);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(0.0f, 0.0f, fac);
			positions[j++] = glm::vec3(r, g, b);

			// triangle of i, i + 1, bottom
			r = (float) rand() / (float) RAND_MAX;
			g = (float) rand() / (float) RAND_MAX;
			b = (float) rand() / (float) RAND_MAX;
			positions[j++] = glm::vec3(fac * std::cos(pi / 3 * i), fac * std::sin(pi / 3 * i), 0.0f);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(pi / 3 * ((i + 1) % 6)), fac * std::sin(pi / 3 * ((i + 1) % 6)), 0.0f);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(0.0f, 0.0f, -fac);
			positions[j++] = glm::vec3(r, g, b);
		}
	}

	void ProcessKeyboard(Object_Movement direction, float deltaTime) {
		float velocity = OBJSPEED * deltaTime;
		switch (direction) {
			case OZPLUS:
				origin += glm::vec3(0.0f, 0.0f, 1.0f) * velocity;
				break;
			case OZMINUS:
				origin += glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
				break;
			case OYPLUS:
				origin += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
				break;
			case OYMINUS:
				origin += glm::vec3(0.0f, -1.0f, 0.0f) * velocity;
				break;
			case OXPLUS:
				origin += glm::vec3(1.0f, 0.0f, 0.0f) * velocity;
				break;
			case OXMINUS:
				origin += glm::vec3(-1.0f, 0.0f, 0.0f) * velocity;
				break;
		}
	}
};

class DecagonalPrism {
public:
	const unsigned int size = 40 * 3 * (3 + 3);
	const unsigned int numPoints = 40 * 3;
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 positions[40 * 6];

	DecagonalPrism() {
		float fac = 0.5;
		float pi = 3.14159265359;
		float r, g, b;
		float r1, g1, b1;
		float r2, g2, b2;

		r1 = (float) rand() / (float) RAND_MAX;
		g1 = (float) rand() / (float) RAND_MAX;
		b1 = (float) rand() / (float) RAND_MAX;
		r2 = (float) rand() / (float) RAND_MAX;
		g2 = (float) rand() / (float) RAND_MAX;
		b2 = (float) rand() / (float) RAND_MAX;

		for (int i = 0, j = 0; i < 10; i++) {
			positions[j++] = glm::vec3(0.0f, 0.0f, fac);
			positions[j++] = glm::vec3(r1, g1, b1);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * i), fac * std::sin(pi / 5 * i), fac);
			positions[j++] = glm::vec3(r1, g1, b1);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * (i + 1)), fac * std::sin(pi / 5 * ((i + 1) % 10)), fac);
			positions[j++] = glm::vec3(r1, g1, b1);

			positions[j++] = glm::vec3(0.0f, 0.0f, -fac);
			positions[j++] = glm::vec3(r2, g2, b2);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * i), fac * std::sin(pi / 5 * i), -fac);
			positions[j++] = glm::vec3(r2, g2, b2);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * (i + 1)), fac * std::sin(pi / 5 * ((i + 1) % 10)), -fac);
			positions[j++] = glm::vec3(r2, g2, b2);

			r = (float) rand() / (float) RAND_MAX;
			g = (float) rand() / (float) RAND_MAX;
			b = (float) rand() / (float) RAND_MAX;
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * i), fac * std::sin(pi / 5 * i), fac);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * (i + 1)), fac * std::sin(pi / 5 * ((i + 1) % 10)), fac);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * i), fac * std::sin(pi / 5 * i), -fac);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * i), fac * std::sin(pi / 5 * i), -fac);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * (i + 1)), fac * std::sin(pi / 5 * ((i + 1) % 10)), -fac);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(pi / 5 * (i + 1)), fac * std::sin(pi / 5 * ((i + 1) % 10)), fac);
			positions[j++] = glm::vec3(r, g, b);
		}
	}

	void ProcessKeyboard(Object_Movement direction, float deltaTime) {
		float velocity = OBJSPEED * deltaTime;
		switch (direction) {
			case OZPLUS:
				origin += glm::vec3(0.0f, 0.0f, 1.0f) * velocity;
				break;
			case OZMINUS:
				origin += glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
				break;
			case OYPLUS:
				origin += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
				break;
			case OYMINUS:
				origin += glm::vec3(0.0f, -1.0f, 0.0f) * velocity;
				break;
			case OXPLUS:
				origin += glm::vec3(1.0f, 0.0f, 0.0f) * velocity;
				break;
			case OXMINUS:
				origin += glm::vec3(-1.0f, 0.0f, 0.0f) * velocity;
				break;
		}
	}
};

class UndecagonalPyramid {
public:
	const unsigned int size = 66 * 3 * (3 + 3);
	const unsigned int numPoints = 66 * 3;
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 positions[66 * 6];

	UndecagonalPyramid() {
		float fac = 0.5;
		float pi = 3.14159265359;
		float r, g, b;
		float r1, g1, b1;

		r1 = (float) rand() / (float) RAND_MAX;
		g1 = (float) rand() / (float) RAND_MAX;
		b1 = (float) rand() / (float) RAND_MAX;

		for (int i = 0, j = 0; i < 11; i++) {
			positions[j++] = glm::vec3(0.0f, 0.0f, 0.0f);
			positions[j++] = glm::vec3(r1, g1, b1);
			positions[j++] = glm::vec3(fac * std::cos(2 * pi / 11 * i), fac * std::sin(2 * pi / 11 * i), 0.0f);
			positions[j++] = glm::vec3(r1, g1, b1);
			positions[j++] = glm::vec3(fac * std::cos(2 * pi / 11 * ((i + 1) % 11)), fac * std::sin(2 * pi / 11 * ((i + 1) % 11)), 0.0f);
			positions[j++] = glm::vec3(r1, g1, b1);

			r = (float) rand() / (float) RAND_MAX;
			g = (float) rand() / (float) RAND_MAX;
			b = (float) rand() / (float) RAND_MAX;
			positions[j++] = glm::vec3(0.0f, 0.0f, -fac);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(2 * pi / 11 * i), fac * std::sin(2 * pi / 11 * i), 0.0f);
			positions[j++] = glm::vec3(r, g, b);
			positions[j++] = glm::vec3(fac * std::cos(2 * pi / 11 * ((i + 1) % 11)), fac * std::sin(2 * pi / 11 * ((i + 1) % 11)), 0.0f);
			positions[j++] = glm::vec3(r, g, b);
		}
	}

	void ProcessKeyboard(Object_Movement direction, float deltaTime) {
		float velocity = OBJSPEED * deltaTime;
		switch (direction) {
			case OZPLUS:
				origin += glm::vec3(0.0f, 0.0f, 1.0f) * velocity;
				break;
			case OZMINUS:
				origin += glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
				break;
			case OYPLUS:
				origin += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
				break;
			case OYMINUS:
				origin += glm::vec3(0.0f, -1.0f, 0.0f) * velocity;
				break;
			case OXPLUS:
				origin += glm::vec3(1.0f, 0.0f, 0.0f) * velocity;
				break;
			case OXMINUS:
				origin += glm::vec3(-1.0f, 0.0f, 0.0f) * velocity;
				break;
		}
	}
};
#endif
