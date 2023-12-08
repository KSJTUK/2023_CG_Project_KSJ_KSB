#include "pch.h"
#include "Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Terrain.h"
#include "Graphics/SkyBox.h"
#include "Graphics/AnimatedModel.h"



#define TEST_PATCHSIZE 20

Renderer::Renderer() { }


Renderer::Renderer(GLFWwindow* window) {
	m_freeCamera = std::make_unique<FreeCamera>(window, glm::vec3{ 10.f,10.f,10.f }, glm::vec3{ -1.f,0.f,0.f });
	m_background = std::make_unique<SkyBox>();
	m_testTerrain = std::make_unique<Terrain>(glm::uvec2{ TEST_PATCHSIZE, TEST_PATCHSIZE });



	m1 = new Animated::Model{};

	m1->LoadModel("Resources/ar15/scene.gltf");


}

Renderer::~Renderer() { }

void Renderer::Update(float deltaTime) {
	m_freeCamera->Update(deltaTime);
}

void Renderer::Render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SHADER->UseProgram(ShaderType::StaticShader);
	m_freeCamera->Render();
	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::BackgroundShader);
	m_freeCamera->Render();
	m_background->Render();
	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::TerrainShader);
	m_freeCamera->Render();
	m_testTerrain->Render();
	SHADER->UnuseProgram();


	SHADER->UseProgram(ShaderType::AnimatedShader);
	m_freeCamera->Render();
	m1->Render();
	SHADER->UnuseProgram();

}
