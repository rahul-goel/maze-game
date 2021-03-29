#ifndef CHARACTER_H
#define CHARACTER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sprite_renderer.h"
#include "circle_renderer.h"

class Character {
	public:
		glm::vec2 Position;
		glm::vec2 Size;
		glm::vec3 EyeColor;
		glm::vec3 BodyColor;
		float CircleRadius;
		Character(glm::vec2 pos, glm::vec2 Size, glm::vec3 eye_color, glm::vec3 body_color);
		void Render(SpriteRenderer *renderer, CircleRenderer *circle_renderer, float intensity = 1.0f);

	private:
		glm::vec2 getCircleCenter();
};

#endif
