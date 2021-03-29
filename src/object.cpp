#include "object.h"
#include "circle_renderer.h"
#include "sprite_renderer.h"
#include "texture.h"

#include <iostream>

Object::Object(glm::vec2 pos, glm::vec2 size, glm::vec3 body_color) {
	this->Position = pos;
	this->Size = size;
	this->CircleRadius = size.x / 4;
	this->BodyColor = body_color;
	this->exist = true;
}

glm::vec2 Object::getCircleCenter() {
	glm::vec2 ret;
	ret.y = this->Position.x *  this->Size.x + this->Size.x / 2;
	ret.x = this->Position.y *  this->Size.y + this->Size.x / 2;
	return ret;
}

void Object::Render(SpriteRenderer *renderer, CircleRenderer * circle_renderer, float intensity) {
	Texture2D tex;
	//circle
	circle_renderer->DrawCircle(tex, this->getCircleCenter(), this->CircleRadius, this->BodyColor, intensity);
}
