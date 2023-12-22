#include "pch.h"
#include "UI.h"
#include "Util/Engine.h"
#include "Graphics/Shader.h"
#include "Graphics/GraphicBuffers.h"
#include "Util/TextureComponent.h"

UI::UI(GLFWwindow* const windowPtr) {
	int windowW{ }, windowH{ };
	glfwGetFramebufferSize(windowPtr, &windowW, &windowH);
	m_windowPtr = windowPtr;
	m_scale = { 1.f / static_cast<float>(windowW), 1.f / static_cast<float>(windowH), 0.f};

	float uiDefaultSize = 100.f;
	auto w = uiDefaultSize;
	auto h = uiDefaultSize;

	m_quad = {
		vertex{ glm::vec3{ -w, h, 0.f }, { }, glm::vec2{ 0.f, 0.f } },
		vertex{ glm::vec3{ -w, -h, 0.f }, { }, glm::vec2{ 0.f, 1.f } },
		vertex{ glm::vec3{ w, h, 0.f }, { }, glm::vec2{ 1.f, 0.f } },
		vertex{ glm::vec3{ w, -h, 0.f }, { }, glm::vec2{ 1.f, 1.f } }
	};

	m_buffers = std::make_unique<GraphicBuffers>();
	m_buffers->Init();
	m_buffers->SetVerticies(m_quad);

	m_texture = std::make_unique<TextureComponent>();
	m_texture->LoadTexture("./textures/test_ui0.jpg", false);
}

UI::~UI() { }

void UI::SetScreenPosition(const glm::vec3& position) {
	int w{ }, h{ };
	glfwGetFramebufferSize(m_windowPtr, &w, &h);
	m_screenPosition.x = position.x / (static_cast<float>(w) / 2.f);
	m_screenPosition.y = position.y / (static_cast<float>(h) / 2.f);
	m_screenPosition.z = 0.f;
}

void UI::ResizedWindow() {
	int w{ }, h{ };
	glfwGetFramebufferSize(m_windowPtr, &w, &h);
	m_scale = { 1.f / static_cast<float>(w), 1.f / static_cast<float>(h), 0.f };
}

void UI::Update(float deltaTime) { }

void UI::Render() {
	m_texture->BindingTextures();

	glm::mat4 transform{ 1.f };
	auto scale = glm::scale(transform, m_scale);
	auto translate = glm::translate(transform, m_screenPosition);
	transform = translate * scale;
	SHADER->GetActivatedShader()->SetUniformMat4("transform", GL_FALSE, &transform[0][0]);

	m_buffers->SetDrawMode(GL_TRIANGLE_STRIP);
	m_buffers->Render();
}

Reticle::Reticle(GLFWwindow* const windowPtr) {
	int windowW{ }, windowH{ };
	glfwGetFramebufferSize(windowPtr, &windowW, &windowH);
	m_windowPtr = windowPtr;
	m_scale = { 1.f / static_cast<float>(windowW), 1.f / static_cast<float>(windowH), 0.f };

	float uiDefaultSize = 30.f;
	float offSet = 3.f;
	auto w = uiDefaultSize;
	auto h = uiDefaultSize;

	m_quad = {
		vertex{ glm::vec3{ -w, 0.f, 0.f }, { }, glm::vec2{ 0.f, 1.f } },
		vertex{ glm::vec3{ -offSet, 0.f, 0.f }, { }, glm::vec2{ 1.f, 1.f } },
		vertex{ glm::vec3{ offSet, 0.f, 0.f }, { }, glm::vec2{ 1.f, 0.f } },
		vertex{ glm::vec3{ w, 0.f, 0.f }, { }, glm::vec2{ 0.f, 0.f } },
		vertex{ glm::vec3{ 0.f, -h, 0.f }, { }, glm::vec2{ 1.f, 1.f } },
		vertex{ glm::vec3{ 0.f, -offSet, 0.f }, { }, glm::vec2{ 1.f, 1.f } },
		vertex{ glm::vec3{ 0.f, offSet, 0.f }, { }, glm::vec2{ 1.f, 1.f } },
		vertex{ glm::vec3{ 0.f, h, 0.f }, { }, glm::vec2{ 1.f, 1.f } },
	};

	m_buffers = std::make_unique<GraphicBuffers>();
	m_buffers->Init();
	m_buffers->SetVerticies(m_quad);
}

Reticle::~Reticle() { }

void Reticle::ResizedWindow() {
	int w{ }, h{ };
	glfwGetFramebufferSize(m_windowPtr, &w, &h);
	m_scale = { 1.f / static_cast<float>(w), 1.f / static_cast<float>(h), 0.f };
}

void Reticle::Update(float deltaTime) { }

void Reticle::Render() {
	glm::mat4 transform{ 1.f };
	transform = glm::scale(transform, m_scale);
	SHADER->GetActivatedShader()->SetUniformInt("noTexture", GL_TRUE);
	SHADER->GetActivatedShader()->SetUniformVec3("uiColor", &glm::vec3{ 0.f, 1.f, 0.f }[0]);
	SHADER->GetActivatedShader()->SetUniformMat4("transform", GL_FALSE, &transform[0][0]);

	m_buffers->SetDrawMode(GL_LINES);
	m_buffers->Render();
}
