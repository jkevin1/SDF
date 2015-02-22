#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 mvp;

out vec2 uv;

void main() {
	uv = vec2(position.x, 1.0 - position.y);
    gl_Position = mvp * vec4(position, 0.0, 1.0);
}