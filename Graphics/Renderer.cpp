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

#define TEST_PATCHSIZE 20

Renderer::Renderer() { }


Renderer::Renderer(GLFWwindow* window) {
	m_freeCamera = std::make_unique<FreeCamera>(window, glm::vec3{ 0.f,0.f,0.f }, glm::vec3{ -1.f,0.f,0.f });
	m_background = std::make_unique<SkyBox>();
	m_testTerrain = std::make_unique<Terrain>(glm::uvec2{ TEST_PATCHSIZE, TEST_PATCHSIZE });

	ar15_model = std::make_shared<Animated::Model>();
	ar15_model->LoadModel("Resources/zombie/scene.gltf");
	

	for (auto i = 0; i < 1; ++i) {
		std::shared_ptr<Animated::AR15> obj = std::make_shared<Animated::AR15>(ar15_model);

		obj->SetAnimationIndex(1);
		obj->SetPosition(glm::vec3{
			10.f, 0.f, 10.f
		});


		m_animatedObjectArr.push_back(obj);

	}

	static_model = std::make_shared<Static::Model>();
	static_model->LoadModel("Resources/pine_tree/scene.gltf");

	m_testLight = std::make_unique<PointLight>();
	m_testLight->SetPosition(glm::vec3{ 20.f, 20.f, 20.f });



	m_zombie = std::make_shared<Animated::Zombie>(ar15_model, m_freeCamera->GetViewPtr(), m_freeCamera->GetProjectionPtr(), m_freeCamera->GetPositionPtr());

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

	m_zombie->Update(deltaTime);
	


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
		//o->Render();
	}

	m_zombie->Render();
	SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::StaticShader);
	m_freeCamera->Render();

	m_testLight->Render();
	glm::mat4 identity{ 1.f };
	static_model->Render(identity);

	SHADER->UnuseProgram();
}