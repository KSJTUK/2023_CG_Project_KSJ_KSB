#version 460 core

in vec3 vs_out_tex;

out vec4 FragColor;

uniform samplerCube cubeMapTexture;

void main(void)
{
	FragColor = texture(cubeMapTexture, vs_out_tex);
}