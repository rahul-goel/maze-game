#ifndef CIRCLE_RENDERER_H
#define CIRCLE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class CircleRenderer {
	public:
		// Constructor (inits shaders/shapes)
		CircleRenderer(Shader &shader);
		// Destructor
		~CircleRenderer();
		// Renders a defined quad textured with given sprite
		void DrawCircle(Texture2D &texture, glm::vec2 position, float radius = 1.0f, glm::vec3 color = glm::vec3(1.0f));
	private:
		// Render state
		Shader shader;
		unsigned int circleVAO;
		// Initializes and configures the quad's buffer and vertex attributes
		void initRenderData();
};

#endif
