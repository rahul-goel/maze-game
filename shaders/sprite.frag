#version 330 core
out vec4 FragColor;

uniform vec3 spriteColor;
uniform vec3 light;

void main() {
	FragColor = vec4(light * spriteColor, 1.0f);
}
