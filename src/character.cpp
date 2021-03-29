#include "character.h"
#include "circle_renderer.h"
#include "sprite_renderer.h"
#include "texture.h"

#include <iostream>

Character::Character(glm::vec2 pos, glm::vec2 size, glm::vec3 eye_color, glm::vec3 body_color) {
	this->Position = pos;
	this->Size = size;
	this->CircleRadius = size.x / 4;
	this->EyeColor = eye_color;
	this->BodyColor = body_color;
}

glm::vec2 Character::getCircleCenter() {
	glm::vec2 ret;
	ret.y = this->Position.x *  this->Size.x + this->Size.x / 3;
	ret.x = this->Position.y *  this->Size.y + this->Size.x / 2;
	return ret;
}

void Character::Render(SpriteRenderer *renderer, CircleRenderer * circle_renderer) {
	Texture2D tex;
	//circle
	circle_renderer->DrawCircle(tex, this->getCircleCenter(), this->CircleRadius, this->BodyColor);
	//rest of body
	glm::vec2 bodyPosition = this->getCircleCenter();
	bodyPosition.x -= this->CircleRadius;
	glm::vec2 bodySize = glm::vec2(2 * CircleRadius, this->Size.y / 2);
	renderer->DrawSprite(tex, bodyPosition, bodySize, 0.0f, this->BodyColor);

	//eyes
	glm::vec2 eyePosition = this->getCircleCenter();
	eyePosition.y -= this->CircleRadius / 2;
	glm::vec2 eyeSize = glm::vec2(CircleRadius / 1.2, CircleRadius / 2);
	renderer->DrawSprite(tex, eyePosition, eyeSize, 0.0f, this->EyeColor);
}
