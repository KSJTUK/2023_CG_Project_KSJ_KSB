#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_Texture;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 M_matrix;
uniform mat4 TIM_matrix;

uniform mat4 VP;

// lighting
out vec3 normal;
out vec2 text_coords;
out vec3 frag_pos;

void main(void)
{
	frag_pos = vec3(M_matrix * vec4(in_Position, 1.0f));
	normal = normalize( mat3(TIM_matrix) * in_Normal);
	text_coords = in_Texture;

	gl_Position = VP * vec4(frag_pos, 1.0f);
}