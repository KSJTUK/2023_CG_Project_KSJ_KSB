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
	ar15_model->LoadModel("Resources/zombie/scene.gltf");



	for(auto i = 0; i < glm::linearRand(1,50); ++i){
		
		std::shared_ptr<Animated::AR15> obj = std::make_shared<Animated::AR15>(ar15_model);

		obj->SetPosition(glm::vec3{
			
			glm::linearRand(-100.f,100.f),
			0.f,
			glm::linearRand(-100.f,100.f)
		});

		obj->SetAnimationIndex(glm::linearRand(0, 9));
		m_animatedObjectArr.push_back(obj);
		
	}


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

Renderer::~Renderer() { }

void Renderer::CollisionTerrain(Animated::Object& obj, float offset) {
	m_testTerrain->MoveHeightPosition(obj.GetPosition(), offset);
}

void Renderer::Update(float deltaTime) {
	m_freeCamera->Update(deltaTime);

	for (const auto& o : m_animatedObjectArr) {
		o->Update(deltaTime);
	}


	//ar15_2->SetPosition(ar15_2->GetPosition() + glm::vec3{ 0.f, 0.f, 30.f * deltaTime });
	//CollisionTerrain(*ar15_2, 1.f);
}

void Renderer::Render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

	for (const auto& o : m_animatedObjectArr) {
		o->Render();
	}

	SHADER->UnuseProgram();

}
