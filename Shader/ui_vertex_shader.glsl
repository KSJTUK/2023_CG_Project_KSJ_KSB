#version 460 core
//--- in_Position: attribute index 0
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec2 in_Texture; //--- 텍스처 변수: attribute position 1
layout (location = 2) in vec3 in_Normal; //--- 정점 노멀 변수: attribute position 2

out vec2 texCoords;

uniform mat4 transform;

void main(void)
{
	texCoords = in_Texture;

	gl_Position = transform * vec4(in_Position, 1.0f);
}