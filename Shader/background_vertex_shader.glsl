#version 460 core

layout(location=0) in vec3 in_Position;

uniform mat4 projection;
uniform mat4 view;

out vec3 vs_out_tex;

void main(void) 
{
	vs_out_tex = in_Position;
	vec4 position = projection * mat4(mat3(view)) * vec4(in_Position, 1.0f);
	gl_Position = position.xyww;
}