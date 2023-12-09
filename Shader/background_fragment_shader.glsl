#version 460 core

in vec3 vs_out_tex;

out vec4 FragColor;

uniform samplerCube cubeMapTexture;
uniform vec3 ambient;

void main(void)
{
	vec3 result = ambient * vec3(texture(cubeMapTexture, vs_out_tex));
	FragColor = vec4(result, 1.0f);
}