#include "pch.h"
#include "Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Terrain.h"
#include "Graphics/SkyBox.h"
#include "Graphics/AnimatedModel.h"
#include "Graphics/StaticModel.h"
#include "Util/Input.h"

#include "Graphics/AR15.h"
#include "Graphics/Zombie.h"
#include "Graphics/Lighting.h"
#include "Graphics/PineTree.h"

#define TEST_PATCHSIZE 20

Renderer::Renderer() { }


Renderer::Renderer(GLFWwindow* window) {
	m_freeCamera = std::make_unique<FreeCamera>(window, glm::vec3{ 0.f,0.f,0.f }, glm::vec3{ -1.f,0.f,0.f });
	m_background = std::make_unique<SkyBox>();
	m_testTerrain = std::make_unique<Terrain>(glm::uvec2{ TEST_PATCHSIZE, TEST_PATCHSIZE });

	ar15_model = std::make_shared<Animated::Model>();
	ar15_model->LoadModel("Resources/ar15/scene.gltf");

	zombie_model = std::make_shared<Animated::Model>();
	zombie_model->LoadModel("Resources/zombie/scene.gltf");

	static_model = std::make_shared<Static::Model>();
	static_model->LoadModel("Resources/pine_tree/scene.gltf");

	

	for (auto i = 0; i < 30; ++i) {
		std::shared_ptr<Animated::Zombie> obj = std::make_shared<Animated::Zombie>(zombie_model, m_freeCamera->GetViewPtr(), m_freeCamera->GetProjectionPtr(), m_freeCamera->GetPositionPtr());
		obj->SetPosition(glm::vec3{
			glm::linearRand(-100.f,100.f),0.f,glm::linearRand(-100.f,100.f)
		});
		obj->SetAnimation(glm::linearRand(0, 9));
		m_animatedObjectArr.push_back(obj);

	}


	for (auto i = 0; i < 100; ++i) {
		std::shared_ptr<Static::PineTree> obj = std::make_shared<Static::PineTree>(static_model, glm::vec3{glm::linearRand(-100.f,100.f),0.f,glm::linearRand(-100.f,100.f)});
		m_staticObjectArr.push_back(obj);

	}

	m_testLight = std::make_unique<PointLight>();
	m_testLight->SetPosition(glm::vec3{ 20.f, 20.f, 20.f });




	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

Renderer::~Renderer() { }

void Renderer::CollisionTerrain(Animated::Object& obj, float offset) {
	m_testTerrain->MoveHeightPosition(obj.GetPosition(), offset);
}

constexpr glm::vec3 RayPos = glm::vec3{ 0.f,0.f,0.f };
constexpr glm::vec3 RayDir = glm::vec3{ 0.f,0.f,1.f };

void Renderer::Update(float deltaTime) {
	m_freeCamera->Update(deltaTime);

	m_background->Update(deltaTime);




	for (auto& o : m_animatedObjectArr) {
		o->Update(deltaTime);
		CollisionTerrain(*o, 1.f);
	}
}

void Renderer::Render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	SHADER->UseProgram(ShaderType::BackgroundShader);
	m_freeCamera->Render();
	m_background->Render();
	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::TerrainShader);
	m_freeCamera->Render();
	m_testLight->SetPosition(glm::vec3{ 20.f, 20.f, 20.f });
	m_testLight->Render();
	m_testTerrain->Render();
	SHADER->UnuseProgram();


	SHADER->UseProgram(ShaderType::AnimatedShader);
	m_freeCamera->Render();
	m_testLight->Render();
	for (auto& o : m_animatedObjectArr) {
		o->Render();
	}

	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::StaticShader);
	m_freeCamera->Render();

	m_testLight->Render();
	
	for (auto& o : m_staticObjectArr) {
		o->Render();
	}

	SHADER->UnuseProgram();
}