#version 450 core
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	vec4 vertices[3] = vec4[3](
		vec4( 0.0, -0.5, 0.0, 1.0),
		vec4( 0.5,  0.5, 0.0, 1.0),
		vec4(-0.5,  0.5, 0.0, 1.0)
	);

	gl_Position = vertices[gl_VertexIndex];
}