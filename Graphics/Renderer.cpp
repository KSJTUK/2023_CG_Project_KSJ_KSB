#include "pch.h"
#include "Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Terrain.h"
#include "Graphics/SkyBox.h"
#include "Graphics/AnimatedModel.h"
#include "Graphics/StaticModel.h"
#include "Util/Input.h"
#include "UI.h"

#include "Graphics/AR15.h"
#include "Graphics/Zombie.h"
#include "Graphics/Lighting.h"
#include "Graphics/PineTree.h"

#define PATCH_SIZE 20

Renderer::Renderer() { }

Renderer::Renderer(GLFWwindow* window) {
	m_mainCamera = std::make_unique<FreeCamera>(window, glm::vec3{ 0.f,0.f,0.f }, glm::vec3{ -1.f,0.f,0.f });
	m_background = std::make_unique<SkyBox>();
	m_terrain = std::make_unique<Terrain>(glm::uvec2{ PATCH_SIZE, PATCH_SIZE });

	ar15_model = std::make_shared<Animated::Model>();
	ar15_model->LoadModel("Resources/ar15/scene.gltf");

	zombie_model = std::make_shared<Animated::Model>();
	zombie_model->LoadModel("Resources/zombie/scene.gltf");

	pineTree_model = std::make_shared<Static::Model>();
	pineTree_model->LoadModel("Resources/pine_tree/scene.gltf");

	for (auto i = 0; i < 20; ++i) {
		std::shared_ptr<Animated::Zombie> obj = std::make_shared<Animated::Zombie>(zombie_model, m_mainCamera->GetViewPtr(),
			m_mainCamera->GetProjectionPtr(), m_mainCamera->GetPositionPtr(), m_mainCamera->GetBasisZPtr());

		obj->SetPosition(glm::vec3{
			glm::linearRand(-100.f,100.f),0.f,glm::linearRand(-100.f,100.f)
		});
		m_animatedObjectArr.push_back(obj);
	}

	for (auto i = 0; i < 1000; ++i) {
		glm::vec3 randPosition{ glm::linearRand(-1000.f,1000.f),0.f,glm::linearRand(-1000.f,1000.f) };
		randPosition.y = m_terrain->GetHeight(randPosition, -1.f);
		std::shared_ptr<Static::PineTree> obj = std::make_shared<Static::PineTree>(pineTree_model, randPosition);
		m_staticObjectArr.push_back(obj);
	}

	m_pointLight = std::make_unique<PointLight>();
	m_pointLight->SetPosition(glm::vec3{ 20.f, 20.f, 20.f });
	
	m_spotLight = std::make_unique<SpotLight>();
	m_globalLight = std::make_unique<DirectionLight>();

	m_testReticle = std::make_unique<Reticle>(window);
	m_ar15 = std::make_unique<Animated::AR15>(ar15_model);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Renderer::~Renderer() { }

void Renderer::CollisionTerrain(Animated::Object& obj, float offset) {
	m_terrain->MoveHeightPosition(obj.GetPosition(), offset);
}

void Renderer::CollisionTerrain(Static::Object& obj, float offset) {
	m_terrain->MoveHeightPosition(obj.GetPosition(), offset);
}

constexpr glm::vec3 RayPos = glm::vec3{ 0.f,0.f,0.f };
constexpr glm::vec3 RayDir = glm::vec3{ 0.f,0.f,1.f };

void Renderer::Update(float deltaTime) {
	for (auto& o : m_staticObjectArr) {
		m_mainCamera->CollisionObject(*o);
	}

	m_mainCamera->Update(deltaTime);
	m_ar15->Update(deltaTime);

	m_ar15->SetPosition(m_mainCamera->GetPosition() + m_mainCamera->GetViewPoint()
		+ (*m_mainCamera->GetBasisXPtr()) * 0.1f + (*m_mainCamera->GetBasisZPtr()) * 0.7f - (*m_mainCamera->GetBasisYPtr()) * 0.2f);
	m_ar15->SetRotateAngle(m_ar15->GetRotateAngle() + m_mainCamera->GetDeltaAngle());

	m_terrain->MoveHeightPosition(m_mainCamera->GetPosition(), 17.f);

	m_spotLight->SetPosition(m_mainCamera->GetPosition());
	m_spotLight->SetDirection(m_mainCamera->GetViewPoint());

	m_globalLight->DayUpdate(deltaTime);
	m_background->SetAmbient(m_globalLight->GetDirLightColor());

	for (auto& o : m_animatedObjectArr) {
		o->Update(deltaTime);
		CollisionTerrain(*o, 1.f);
	}
}

void Renderer::Render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	SHADER->UseProgram(ShaderType::BackgroundShader);
	m_mainCamera->Render();
	m_background->Render();
	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::TerrainShader);
	m_mainCamera->Render();
	m_pointLight->Render();
	m_globalLight->Render();
	m_spotLight->Render();
	m_terrain->Render();
	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::AnimatedShader);
	m_mainCamera->Render();
	m_pointLight->Render();
	m_globalLight->Render();
	m_spotLight->Render();
	m_ar15->Render();
	for (auto& o : m_animatedObjectArr) {
		o->Render();
	}

	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::StaticShader);
	m_mainCamera->Render();

	m_pointLight->Render();
	m_spotLight->Render();
	m_globalLight->Render();
	
	for (auto& o : m_staticObjectArr) {
		o->Render();
	}
	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::UIShader);
	m_testReticle->Render();
	SHADER->UnuseProgram();
}