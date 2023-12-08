#version 460 core

layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec2 in_Texture; //--- 텍스처 변수: attribute position 1
layout (location = 2) in vec3 in_Normal; //--- 정점 노멀 변수: attribute position 2

out vec2 vs_out_tex;
out vec3 vs_out_normal;
out vec3 vs_out_fragPosition;

uniform mat4 view;

void main(void)
{
	gl_Position = view * vec4(in_Position, 1.0f);
	vs_out_tex = in_Texture;
	vs_out_normal = in_Normal;
	vs_out_fragPosition = in_Position;
}