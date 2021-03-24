#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 inColor;

out vec3 outColor;

void main() {
	gl_Position = position;
	outColor = inColor;
}
