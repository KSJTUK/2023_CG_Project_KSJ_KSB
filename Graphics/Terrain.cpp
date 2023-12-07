#include "pch.h"
#include "Terrain.h"
#include "Graphics/GraphicBuffers.h"
#include "Util/TextureComponent.h"
#include "Graphics/Shader.h"

Terrain::Terrain(const glm::uvec2& mapSize) : m_terrainMapSize{ mapSize } {
	// 먼저 하이트맵의 텍스쳐를 불러옴
	SHADER->UseProgram(ShaderType::TerrainShader);

	m_textureComponent = std::make_unique<TextureComponent>();
	m_textureHeight = m_textureComponent->LoadHeightMap("./Textures/height1.png", m_yScale, m_yShift, false);
	auto info = m_textureComponent->GetTextureInfo(HEIGHT_MAP);
	m_terrainScale = { info.width, info.height };
	m_textureComponent->LoadTexture("./Textures/terrain1.png", false);

	m_vertexBuffer = std::make_unique<GraphicBuffers>();
	m_vertexBuffer->Init();

	CreateTerrainMeshMap();
	m_vertexBuffer->SetVerticies(m_verticies);

	Init();

	SHADER->UnuseProgram();
}

Terrain::~Terrain() { }

void Terrain::CreateTerrainMeshMap() {
	auto heightMapInfo = m_textureComponent->GetTextureInfo(HEIGHT_MAP);
	float tileWidth = heightMapInfo.width / static_cast<float>(m_terrainMapSize.x);
	float tileHeight = heightMapInfo.height / static_cast<float>(m_terrainMapSize.y);
	float left = (-heightMapInfo.width / 2.f);
	float bottom = (-heightMapInfo.height / 2.f);
	glm::vec3 terrainNorm{ 0.f, 1.f, 0.f };
	// xz평면 상에 지형을 그려줄 큐브 메쉬들의 정점을 생성함
	for (unsigned int z = 0; z < m_terrainMapSize.y; ++z) {
		for (unsigned int x = 0; x < m_terrainMapSize.x; ++x) {
			// 중앙을 0,0으로 잡고 왼쪽에서 부터 오른쪽으로 만듬
			// 공식은 이미지 너비의 반을 hw, 높이의 반을 hh라 하면 
			// i는 타일맵 으로 생각할때 타일 맵의 번호
			// -hw + w * ((i + 1 or + 0) / mapSize) -> hw를 왼쪽 오른쪽이라 생각하면 각 타일의 너비 높이는 w / mapsize.x, h / mapsize.z 이므로
			// left + (x * (w / mapsize.x), bottom + (z * (h / mapsize.z))
			// xz평면 상에 생성할 것이므로 y좌표는 항상 0
			// 텍스쳐 좌표는 정규화된 좌표이므로 0,1 사이에 존재해야함 따라서 
			// u,v = x / mapsize.x, z / mapsize.z
			vertex p0{
				glm::vec3{
					left + static_cast<float>(x * tileWidth),
					0.f,
					bottom + static_cast<float>(z * tileHeight)
				},
				terrainNorm,
				glm::vec2{
					static_cast<float>(x) / m_terrainMapSize.x, static_cast<float>(z) / m_terrainMapSize.y
				},
			};

			vertex p1{
				glm::vec3{
					left + static_cast<float>((x + 1) * tileWidth),
					0.f,
					bottom + static_cast<float>((z * tileHeight))
				},
				terrainNorm,
				glm::vec2{
					static_cast<float>(x + 1) / m_terrainMapSize.x, static_cast<float>(z) / m_terrainMapSize.y
				},
			};

			vertex p2{
				glm::vec3{
					left + static_cast<float>(x * tileWidth),
					0.f,
					bottom + static_cast<float>((z + 1) * tileHeight)
				},
				terrainNorm,
				glm::vec2{
					static_cast<float>(x) / m_terrainMapSize.x, static_cast<float>(z + 1) / m_terrainMapSize.y
				},
			};

			vertex p3{
				glm::vec3{
					left + static_cast<float>((x + 1) * tileWidth),
					0.f,
					bottom + static_cast<float>((z + 1) * tileHeight)
				},
				terrainNorm,
				glm::vec2{
					static_cast<float>(x + 1) / m_terrainMapSize.x, static_cast<float>(z + 1) / m_terrainMapSize.y
				},
			};

			m_verticies.push_back(p0);
			m_verticies.push_back(p2);
			m_verticies.push_back(p1);
			m_verticies.push_back(p3);
		}
	}

	std::cout << "creating terrain success : \n\tterrain patch size{ " << m_terrainMapSize.x << " x " << m_terrainMapSize.x << " }, "
		<< "\n\tterrain vertex size{ " << m_verticies.size() << " }\n";
}

float Terrain::GetHeight(const glm::vec3& position, float offset) {
	int32 xPos = static_cast<int32>(position.x) + m_terrainScale.x / 2;
	int32 zPos = static_cast<int32>(position.z) + m_terrainScale.y / 2;

	if (xPos < 0 or xPos > m_textureHeight[0].size() - 1) {
		return -_FMAX;
	}

	if (zPos < 0 or zPos > m_textureHeight.size() - 1) {
		return -_FMAX;
	}

	return m_textureHeight[zPos][xPos] + offset;
}

void Terrain::MoveHeightPosition(glm::vec3& position, float offset) {
	int32 xPos = static_cast<int32>(position.x) + m_terrainScale.x / 2;
	int32 zPos = static_cast<int32>(position.z) + m_terrainScale.y / 2;

	if (xPos < 0 or xPos > m_textureHeight[0].size()) {
		position.y = 0.f;
		return;
	}

	if (zPos < 0 or zPos > m_textureHeight.size()) {
		position.y = 0.f;
		return;
	}

	position.y = m_textureHeight[zPos][xPos] + offset;
}

void Terrain::SetMeterials() {
	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.position", &glm::vec3{ 0.f, 3.f, 0.f }[0]);
	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.ambient", &glm::vec3{ 0.5f }[0]);
	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.diffuse", &glm::vec3{ 0.5f }[0]);
	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.specular", &glm::vec3{ 1.f }[0]);
	SHADER->GetActivatedShader()->SetUniformFloat("pointLight.constant", 1.f);
	SHADER->GetActivatedShader()->SetUniformInt("heightMap", 0);
	SHADER->GetActivatedShader()->SetUniformInt("meterials.heightMapTexture", 1);
	SHADER->GetActivatedShader()->SetUniformVec3("meterials.specular", &glm::vec3{ 0.1f }[0]);
	SHADER->GetActivatedShader()->SetUniformFloat("meterials.shininess", 32.f);
}

void Terrain::Init() {
	/*TERRAINSHADER->SetUniformFloat("yScale", m_yScale);
	TERRAINSHADER->SetUniformFloat("yShift", m_yShift);*/
}

void Terrain::Update(float deltaTime) {

}

void Terrain::Render() {
	SHADER->UseProgram(ShaderType::TerrainShader);
	SetMeterials();


	m_textureComponent->BindingTexture(HEIGHT_MAP);
	m_textureComponent->BindingTexture(TERRAIN_TEX);

	m_vertexBuffer->SetDrawMode(GL_PATCHES);
	m_vertexBuffer->SetPatchParameters(QUAD_PATCH);
	m_vertexBuffer->Render();

	SHADER->UnuseProgram();
}
