#ifndef CHARACTER_H
#define CHARACTER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sprite_renderer.h"

class Character {
	public:
		glm::vec2 Position;
		glm::vec2 Size;
		Character(glm::vec2 pos, glm::vec2 Size);
		void Render(SpriteRenderer *renderer);
};

#endif
