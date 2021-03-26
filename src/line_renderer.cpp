#include "line_renderer.h"
#include <iostream>


LineRenderer::LineRenderer(Shader &shader) {
	this->shader = shader;
	this->initRenderData();
}

LineRenderer::~LineRenderer() {
	glDeleteVertexArrays(1, &this->lineVAO);
}

void LineRenderer::DrawLine(glm::vec2 position_start, glm::vec2 position_end, glm::vec3 color, float intensity) {
	// prepare transformations
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position_start, 0.0f));

	// scale
	glm::vec3 size = glm::vec3(position_end - position_start, 1.0f);
	model = glm::scale(model, size);

	this->shader.SetMatrix4("model", model);

	// render textured quad
	this->shader.SetVector3f("spriteColor", color);

	// light
	glm::vec3 light = glm::vec3(intensity, intensity, intensity);
	this->shader.SetVector3f("light", light);

	// glActiveTexture(GL_TEXTURE0);
	// texture.Bind();

	glBindVertexArray(this->lineVAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void LineRenderer::initRenderData() {
	// configure VAO/VBO
	unsigned int VBO;
	float vertices[] = {
		// pos
		0.0f, 0.0f,
		1.0f, 1.0f,
	};

	glGenVertexArrays(1, &this->lineVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->lineVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
