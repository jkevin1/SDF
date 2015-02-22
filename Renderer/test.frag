#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D sdf;
uniform int mode;
 
void main() {
	color = color = texture(sdf, uv);
	if (mode == 1) {
		if (color.r < 0.5) discard;
		color = vec4(1.0, 1.0, 1.0, 1.0);
    } else if (mode == 2) {
		if (color.r < 0.4) discard;
		if (color.r < 0.5) {
			color = vec4(0.0, 0.0, 0.0, 1.0);
		} else {
			color = vec4(1.0, 1.0, 1.0, 1.0);
		}
    }
}
