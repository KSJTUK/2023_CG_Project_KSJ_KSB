#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_Texture;
layout (location = 2) in vec3 in_Normal;

out vec2 vs_out_tex;

uniform mat4 transform;

void main(void)
{
	vs_out_tex = in_Texture;
	gl_Position = transform * vec4(in_Position, 1.0f);
}