#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_Texture;
layout (location = 2) in vec3 in_Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

// lighting
out vec3 vs_out_normal;
out vec2 vs_out_tex;
out vec3 fragPosition;

void main(void)
{
	fragPosition = vec3(transform * vec4(in_Position, 1.0f));
	vs_out_normal = mat3(transpose(inverse(transform))) * in_Normal;
	vs_out_tex = in_Texture;

	gl_Position = projection * view * vec4(fragPosition, 1.0f);
}