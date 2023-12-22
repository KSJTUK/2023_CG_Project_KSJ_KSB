#pragma once

enum TerrainTex {
	HEIGHT_MAP = 0,
	TERRAIN_TEX
};

enum PatchParam {
	TRIANGLE_PATCH = 3,
	QUAD_PATCH
};

class Terrain {
public:
	Terrain(const glm::uvec2& mapSize);
	~Terrain();

private:
	glm::uvec2 m_terrainMapSize{ };
	std::vector<vertex> m_verticies{ };

	std::unique_ptr<class TextureComponent> m_textureComponent{ };

	std::unique_ptr<class GraphicBuffers> m_vertexBuffer{ };

	glm::uvec2 m_terrainScale{ };

	std::vector<std::vector<float>> m_textureHeight{ };

	const float m_yScale{ 64.f };
	const float m_yShift{ 64.f / 2.f };

private:
	void CreateTerrainMeshMap();

public:
	float GetHeight(const glm::vec3& position, float offset);
	void MoveHeightPosition(glm::vec3& position, float offset);
	void SetMeterials();

public:
	void Init();
	void Update(float deltaTime);
	void Render();
};