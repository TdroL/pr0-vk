#version 450

layout (location = 0) out vec2 outUV;

out gl_PerVertex {
	vec4 gl_Position;
};

vec2 positions[3] = vec2[](
	vec2(-1.0, -1.0),
	vec2(-1.0,  3.0),
	vec2( 3.0, -1.0)
);

void main() {
	outUV = (positions[gl_VertexIndex] + 1.0) / 2.0;
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
