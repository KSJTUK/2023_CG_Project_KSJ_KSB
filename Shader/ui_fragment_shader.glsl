#version 460 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D uiImage;
uniform vec3 uiColor;
uniform int noTexture;

const int FALSE = 0;

void main(void)
{
	if (noTexture != FALSE) {
		FragColor = vec4(uiColor, 1.0f);
	} else {
		FragColor = vec4(vec3(texture(uiImage, texCoords)) * uiColor, 1.0f);
	}
}