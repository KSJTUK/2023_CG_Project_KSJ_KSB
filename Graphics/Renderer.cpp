#include "pch.h"
#include "Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Terrain.h"
#include "Graphics/SkyBox.h"
#include "Graphics/AnimatedModel.h"

#include "Graphics/AR15.h"


#define TEST_PATCHSIZE 20

Renderer::Renderer() { }


Renderer::Renderer(GLFWwindow* window) {
	m_freeCamera = std::make_unique<FreeCamera>(window, glm::vec3{ 10.f,10.f,10.f }, glm::vec3{ -1.f,0.f,0.f });
	m_background = std::make_unique<SkyBox>();
	m_testTerrain = std::make_unique<Terrain>(glm::uvec2{ TEST_PATCHSIZE, TEST_PATCHSIZE });

	ar15_model = std::make_shared<Animated::Model>();


	ar15_model->LoadModel("Resources/ar15/scene.gltf");

	ar15 = new Animated::AR15(ar15_model);
	ar15->SetAnimationIndex(6);

	ar15_1 = new Animated::AR15(ar15_model);
	ar15_1->SetAnimationIndex(8);
	ar15_1->SetPosition(glm::vec3{ 0.f,0.f,30.f });

	ar15_2 = new Animated::AR15(ar15_model);
	ar15_2->SetAnimationIndex(3);
	ar15_2->SetPosition(glm::vec3{ 0.f,0.f,-30.f });


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

Renderer::~Renderer() { }

void Renderer::Update(float deltaTime) {
	m_freeCamera->Update(deltaTime);
	ar15->Update(deltaTime);
	ar15_1->Update(deltaTime);
	ar15_2->Update(deltaTime);
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
	ar15->Render();
	ar15_1->Render();
	ar15_2->Render();
	SHADER->UnuseProgram();

}
