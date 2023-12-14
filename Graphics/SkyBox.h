#pragma once

class SkyBox {
public:
	SkyBox();
	~SkyBox();

private:
	std::unique_ptr<class CubeMapTexture> m_textures{ };

	std::vector<glm::vec3> m_verticies{ };

	const int m_drawMode{ GL_TRIANGLES };

	glm::mat4 m_transformMat{ };

	unsigned int m_VAO{ };
	unsigned int m_VBO{ };

	glm::vec3 m_ambient{ 1.f };
	float m_lightDir{ 1.f };
	float m_timeSpeed{ 0.001f };

public:
	void Update(float deltaTime);
	void Render();
};