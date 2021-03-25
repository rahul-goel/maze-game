#include "character.h"
#include "sprite_renderer.h"
#include "texture.h"

#include <iostream>

Character::Character(glm::vec2 pos, glm::vec2 size) {
	this->Position = pos;
	this->Size = size;
}

void Character::Render(SpriteRenderer *renderer) {
	Texture2D tex;
	renderer->DrawSprite(tex, this->Position, this->Size, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
}
