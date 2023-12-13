#version 460 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoords;
layout(location = 3) in ivec4 bone_ids;     // INT pointer
layout(location = 4) in vec4 weights;

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;

uniform mat4 VP;

uniform mat4 M_matrix;
uniform mat4 TIM_matrix;


const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main()
{
	mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];
			
	vec4 boned_position = bone_transform * vec4(in_Position, 1.0); // transformed by bones

	normal = normalize(vec3(TIM_matrix * (bone_transform * vec4(in_Normal, 0.0))));

	fragPos = vec3(M_matrix * boned_position);
	texCoords = in_TexCoords;

	gl_Position = VP * M_matrix * boned_position;
}