#include "circle_renderer.h"
#include <iostream>


CircleRenderer::CircleRenderer(Shader &shader) {
	this->shader = shader;
	this->initRenderData();
}

CircleRenderer::~CircleRenderer() {
	glDeleteVertexArrays(1, &this->circleVAO);
}

void CircleRenderer::DrawCircle(Texture2D &texture, glm::vec2 position, float radius, glm::vec3 color, float intensity) {
	// prepare transformations
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

	model = glm::scale(model, glm::vec3(radius, radius, 1.0f)); // last scale

	this->shader.SetMatrix4("model", model);

	// render textured quad
	this->shader.SetVector3f("spriteColor", color);
	this->shader.SetVector3f("light", glm::vec3(intensity, intensity, intensity));

	// glActiveTexture(GL_TEXTURE0);
	// texture.Bind();

	glBindVertexArray(this->circleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glBindVertexArray(0);
}

void CircleRenderer::initRenderData() {
	// configure VAO/VBO
	unsigned int VBO;
	const int num_tri = 36;
	const int deg = 360 / num_tri;
	const float PI = 3.14159;

	float vertices[num_tri * 3 * 2];
	int k = 0;
	for (int i = 0; i < num_tri; i++) {
		int cur = deg * i;
		int next = cur + deg;
		vertices[k++] = 0.0f;
		vertices[k++] = 0.0f;
		vertices[k++] = cos(cur * PI / 180);
		vertices[k++] = sin(cur * PI / 180);
		vertices[k++] = cos(next * PI / 180);
		vertices[k++] = sin(next * PI / 180);
	}

	glGenVertexArrays(1, &this->circleVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->circleVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
