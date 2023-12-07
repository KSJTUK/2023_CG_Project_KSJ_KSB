#include "pch.h"
#include "Graphics/GraphicBuffers.h"

GraphicBuffers::GraphicBuffers() : m_drawMode{ GL_TRIANGLES } { }

GraphicBuffers::~GraphicBuffers() {
	glDeleteVertexArrays(1, &m_vertexArray);
	glDeleteBuffers(1, &m_vertexBuffer);
}

void GraphicBuffers::Init() {
	// VAO 객체 생성 및 바인드
	// VBO 객체 생성 및 바인드
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);

	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
}

void GraphicBuffers::ResetVerticies(const std::vector<Vertex>& verticies) {
	m_vertexDataSize = verticies.size();

	glDeleteVertexArrays(1, &m_vertexArray);

	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);

	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, m_vertexDataSize * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	// location 0번에 Vertex객체의 position정보를 넘겨줌
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// location 1번에 Vertex객체의 texture정보를 넘겨줌
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
	glEnableVertexAttribArray(1);

	// location 2번에 Vertex객체의 normal정보를 넘겨줌
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
}

void GraphicBuffers::SetVerticies(const std::vector<Vertex>& verticies) {
	m_vertexDataSize = verticies.size();

	// Vertex객체의 정보를 VBO에 넘겨줌
	glBindVertexArray(m_vertexArray);
	glBufferData(GL_ARRAY_BUFFER, m_vertexDataSize * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	// location 0번에 Vertex객체의 position정보를 넘겨줌
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// location 1번에 Vertex객체의 texture정보를 넘겨줌
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
	glEnableVertexAttribArray(1);

	// location 2번에 Vertex객체의 normal정보를 넘겨줌
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void GraphicBuffers::SetPatchParameters(int numOfPatches) {
	glPatchParameteri(GL_PATCH_VERTICES, numOfPatches);
}

void GraphicBuffers::BindingTexture(const uint32& textureID) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void GraphicBuffers::SetDrawMode(unsigned int mode) {
	m_drawMode = mode;
}

void GraphicBuffers::Render() {
	// shaderProgram 에서 UseProgram을 활성화 했다는 가정하에 수행
	glBindVertexArray(m_vertexArray);
	glDrawArrays(m_drawMode, 0, uint32(m_vertexDataSize));
	glBindVertexArray(0); // Array 바인드 해제
	glBindTexture(GL_TEXTURE_2D, 0);
}
