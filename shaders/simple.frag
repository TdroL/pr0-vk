#version 450

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

float circle(vec2 position, float r, vec2 uv) {
	return clamp(r * r - dot(uv - position, uv - position), 0.0, 1.0);
}

float aastep(float threshold, float value) {
	float afwidth = length(vec2(dFdx(value), dFdy(value))) * 0.70710678118654757;
	return smoothstep(threshold-afwidth, threshold+afwidth, value);
}

void main() {
	vec2 screen = 1.0 / abs(vec2(dFdx(inUV.x), dFdy(inUV.y)));
	float r = min(screen.x, screen.y) / 2.0;

	vec2 uv = inUV * screen;

	float result = circle(screen / 2, r, uv);

	outColor = vec4(1.0, 1.0 - aastep(0.5, result).xx, 1.0);
}
