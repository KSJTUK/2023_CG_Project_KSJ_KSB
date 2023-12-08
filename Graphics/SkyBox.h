#pragma once

class SkyBox {
public:
	SkyBox();
	~SkyBox();

private:
	std::unique_ptr<class GraphicBuffers> m_buffer{ };
	std::unique_ptr<class CubeMapTexture> m_textures{ };

	std::vector<glm::vec3> m_verticies{ };

	const int m_drawMode{ GL_TRIANGLES };

	glm::mat4 m_transformMat{ };

	unsigned int m_VAO{ };
	unsigned int m_VBO{ };

public:
	void Render();
};