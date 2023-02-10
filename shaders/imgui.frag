#version 450

layout (set = 0, binding = 0) uniform texture2D fontTex;
layout (set = 1, binding = 0) uniform sampler fontSampler;

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec4 inColor;

layout (location = 0) out vec4 outColor;

void main() {
	outColor = inColor * texture(sampler2D(fontTex, fontSampler), inUV);
}
