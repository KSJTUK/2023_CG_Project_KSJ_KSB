#include "pch.h"
#include "Camera.h"


#include "Graphics/Shader.h"


Camera::Camera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT) : m_window(window), m_eye(EYE), m_at(AT){
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	m_aspect = static_cast<float>(width) / static_cast<float>(height);


	m_basisZ = glm::normalize(-m_at);
	m_basisX = glm::normalize(glm::cross(m_up, m_basisZ));
	m_basisY = glm::cross(m_basisZ, m_basisX);

}

FreeCamera::FreeCamera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT) :Camera{ window, EYE, AT }
{
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Render(){
	


	auto p = glm::perspective(m_fovY, m_aspect, m_nearZ, m_farZ);
	auto v = glm::lookAt(m_eye, m_eye + m_at, m_up);
	SHADER->GetActivatedShader()->SetUniformMat4("projection", GL_FALSE, &p[0][0]);
	SHADER->GetActivatedShader()->SetUniformMat4("view", GL_FALSE, &v[0][0]);




}

void FreeCamera::Update(float DeltaTime){
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	m_aspect = static_cast<float>(width) / static_cast<float>(height);




}
