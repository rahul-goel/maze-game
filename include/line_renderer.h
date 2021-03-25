#ifndef LINE_RENDERER_H
#define LINE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class LineRenderer {
	public:
		// Constructor (inits shaders/shapes)
		LineRenderer(Shader &shader);
		// Destructor
		~LineRenderer();
		// Renders a defined quad textured with given sprite
		void DrawLine(glm::vec2 position_start, glm::vec2 position_end, glm::vec3 color = glm::vec3(1.0f));
	private:
		// Render state
		Shader shader;
		unsigned int lineVAO;
		// Initializes and configures the quad's buffer and vertex attributes
		void initRenderData();
};

#endif
