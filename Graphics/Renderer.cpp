#include "pch.h"
#include "Renderer.h"
#include "Graphics/Shader.h"

Renderer::Renderer(GLFWwindow* window) {
	m_freeCamera = std::make_unique<FreeCamera>(window, glm::vec3{ 10.f,10.f,10.f }, glm::vec3{ -1.f,0.f,0.f });
}

void Renderer::Update(float DeltaTime) {
}

void Renderer::Render() {
	SHADER->UseProgram(ShaderType::StaticShader);
	m_freeCamera->Render();
	SHADER->UnuseProgram();
}
