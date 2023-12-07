#pragma once

// VAO, VBO, EBO를 바인딩해서 삼각형을 그려줄 클래스
class GraphicBuffers {
public:
	GraphicBuffers();
	~GraphicBuffers();

private:
	// Vertex Buffer Object ID
	unsigned int m_vertexBuffer{ };

	// Vertex Array Object ID
	unsigned int m_vertexArray{ };

	size_t m_vertexDataSize{ };
	size_t m_indexDataSize{ };

	// 그리기 모드 점, 선, 삼각형 등
	// default값은 삼각형(생성자에서 설정)
	unsigned int m_drawMode{ };

public:
	void Init();

	void ResetVerticies(const std::vector<Vertex>& verticies);

	void SetVerticies(const std::vector<Vertex>& verticies);
	void SetPatchParameters(int numOfPatches);
	void BindingTexture(const uint32& textureID);

	void SetDrawMode(unsigned int mode);
	void Render();
};