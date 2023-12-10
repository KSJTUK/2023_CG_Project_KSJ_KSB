#include "pch.h"
#include "Camera.h"
#include "Graphics/Shader.h"
#include "Util/Input.h"

Camera::Camera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT) : m_window(window), m_eye(EYE), m_at(AT) {
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	m_aspect = static_cast<float>(width) / static_cast<float>(height);


	m_basisZ = glm::normalize(-m_at);
	m_basisX = glm::normalize(glm::cross(m_up, m_basisZ));
	m_basisY = glm::cross(m_basisZ, m_basisX);

	m_projection = glm::perspective(m_fovY, m_aspect, m_nearZ, m_farZ);
}

FreeCamera::FreeCamera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT) :Camera{ window, EYE, AT } {
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::Render(){
	SHADER->GetActivatedShader()->SetUniformMat4("projection", GL_FALSE, &m_projection[0][0]);
	SHADER->GetActivatedShader()->SetUniformMat4("view", GL_FALSE, &m_view[0][0]);
	SHADER->GetActivatedShader()->SetUniformMat4("VP", GL_FALSE, &(m_projection * m_view)[0][0]);
}

constexpr auto CameraSpeed = 20.f;
constexpr auto MOUSE_SENSITIVE = 0.1f;


void FreeCamera::Update(float DeltaTime){
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	m_aspect = static_cast<float>(width) / static_cast<float>(height);

	if (Input::GetInstance()->GetKey(GLFW_KEY_W) == KEY_STATE::PRESS) {
		m_eye -= m_basisZ * DeltaTime * CameraSpeed;
	}


	if (Input::GetInstance()->GetKey(GLFW_KEY_S) == KEY_STATE::PRESS) {
		m_eye += m_basisZ * DeltaTime * CameraSpeed;
	}


	if (Input::GetInstance()->GetKey(GLFW_KEY_A) == KEY_STATE::PRESS) {
		m_eye -= m_basisX * DeltaTime * CameraSpeed;
	}


	if (Input::GetInstance()->GetKey(GLFW_KEY_D) == KEY_STATE::PRESS) {
		m_eye += m_basisX * DeltaTime * CameraSpeed;
	}

	if (Input::GetInstance()->GetKey(GLFW_KEY_Q) == KEY_STATE::PRESS) {
		m_eye -= m_basisY * DeltaTime * CameraSpeed;
	}


	if (Input::GetInstance()->GetKey(GLFW_KEY_E) == KEY_STATE::PRESS) {
		m_eye += m_basisY * DeltaTime * CameraSpeed;

	}


	float2 deltaMouse = Input::GetInstance()->GetDeltaMouse();

	if (!(deltaMouse.x == 0 and deltaMouse.y == 0)) {

		glm::mat4 cameraRotate{ 1.f };


		cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.y * MOUSE_SENSITIVE), m_basisX);
		cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.x * MOUSE_SENSITIVE), m_basisY);

		m_at = glm::normalize(glm::vec3(cameraRotate * glm::vec4(m_at, 1.f)));


		m_basisZ = glm::normalize(-m_at);
		m_basisX = glm::normalize(glm::cross(m_up, m_basisZ));
		m_basisY = glm::cross(m_basisZ, m_basisX);
	}

	m_view = glm::lookAt(m_eye, m_eye + m_at, m_up);
}
