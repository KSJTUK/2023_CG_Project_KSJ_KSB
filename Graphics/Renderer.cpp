#include "pch.h"
#include "Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Terrain.h"
#include "Graphics/SkyBox.h"
#include "Graphics/AnimatedModel.h"
#include "Graphics/StaticModel.h"

#include "Graphics/AR15.h"

#define TEST_PATCHSIZE 20

Renderer::Renderer() { }


Renderer::Renderer(GLFWwindow* window) {
	m_freeCamera = std::make_unique<FreeCamera>(window, glm::vec3{ 10.f,10.f,10.f }, glm::vec3{ -1.f,0.f,0.f });
	m_background = std::make_unique<SkyBox>();
	m_testTerrain = std::make_unique<Terrain>(glm::uvec2{ TEST_PATCHSIZE, TEST_PATCHSIZE });

	ar15_model = std::make_shared<Animated::Model>();
	ar15_model->LoadModel("Resources/zombie/scene.gltf");
	

	for (auto i = 0; i < 50; ++i) {
		std::shared_ptr<Animated::AR15> obj = std::make_shared<Animated::AR15>(ar15_model);


		obj->SetAnimationIndex(glm::linearRand(0, 9));
		obj->SetPosition(glm::vec3{
			glm::linearRand(0.f,100.f),
			0.f,
			glm::linearRand(-500.f ,-600.f)
		});


		m_animatedObjectArr.push_back(obj);

	}

	//m_testModel = std::make_unique<Model>("./tree02.obj", "./Textures/tree02_fall.png");
	//m_testModel->Init();

	static_model = std::make_shared<Static::Model>();
	static_model->LoadModel("Resources/pine_tree/scene.gltf");



	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

Renderer::~Renderer() { }

void Renderer::CollisionTerrain(Animated::Object& obj, float offset) {
	m_testTerrain->MoveHeightPosition(obj.GetPosition(), offset);
}

void Renderer::Update(float deltaTime) {
	m_freeCamera->Update(deltaTime);

	m_background->Update(deltaTime);

	for (auto& zombie : m_animatedObjectArr) {
		zombie->SetPosition(zombie->GetPosition() + glm::vec3{ 0.f, 0.f, 30.f * deltaTime });
		CollisionTerrain(*zombie, 1.f);
	}


	for (auto& o : m_animatedObjectArr) {
		o->Update(deltaTime);
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
	m_testTerrain->Render();
	SHADER->UnuseProgram();


	SHADER->UseProgram(ShaderType::AnimatedShader);
	m_freeCamera->Render();
	for (auto& o : m_animatedObjectArr) {
		o->Render();
	}
	SHADER->UnuseProgram();

	//SHADER->UseProgram(ShaderType::StaticShader);
	//m_freeCamera->Render();
	//SHADER->GetActivatedShader()->SetUniformMat4("transform", GL_FALSE, &glm::mat4{ 1.f } [0] [0]);
	//SHADER->GetActivatedShader()->SetUniformInt("meterials.diffuse", 0);
	//SHADER->GetActivatedShader()->SetUniformInt("noTextureID", 0);
	//SHADER->GetActivatedShader()->SetUniformVec3("pointLight.position", &glm::vec3{ 0.f, 5.f, 0.f } [0] );
	//SHADER->GetActivatedShader()->SetUniformVec3("pointLight.ambient", &glm::vec3{ 1.f } [0] );
	//SHADER->GetActivatedShader()->SetUniformVec3("pointLight.diffuse", &glm::vec3{ 1.f } [0] );
	//SHADER->GetActivatedShader()->SetUniformVec3("objectColor", &glm::vec3{ 1.f } [0] );
	//SHADER->GetActivatedShader()->SetUniformVec3("pointLight.specular", &glm::vec3{ 1.f } [0] );
	//SHADER->GetActivatedShader()->SetUniformFloat("pointLight.constant", 1.f);
	//SHADER->GetActivatedShader()->SetUniformVec3("meterials.specular", &glm::vec3{ 0.1f } [0] );
	//SHADER->GetActivatedShader()->SetUniformFloat("meterials.shininess", 32.f);
	//glEnable(GL_BLEND);
	//glDisable(GL_CULL_FACE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//m_testModel->BindingTexture(0);
	//m_testModel->Render();
	//glDisable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	//SHADER->UnuseProgram();

	SHADER->UseProgram(ShaderType::StaticShader);
	m_freeCamera->Render();

	glm::mat4 identity{ 1.f };
	static_model->Render(identity);

	SHADER->UnuseProgram();
}
