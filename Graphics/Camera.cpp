#include "pch.h"
#include "Camera.h"
#include "Graphics/Shader.h"
#include "Util/Input.h"
#include "Object.h"

constexpr auto CameraSpeed = 20.f;
constexpr auto MOUSE_SENSITIVE = 0.1f;

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
	float halfWidth{ 6.f / 2.f };
	float height{ 17.f };
	m_boxSize.first = { glm::vec3{ halfWidth, height, halfWidth } };
	m_boxSize.second = { glm::vec3{ halfWidth, 0.f, halfWidth} };
	m_boundingBox.first = m_eye - m_boxSize.first;
	m_boundingBox.second = m_eye + m_boxSize.second;
}

FreeCamera::~FreeCamera() {
}

bool FreeCamera::CollisionObject(Animated::Object& obj) {
	auto BB = obj.GetBoundingBox();
	auto min = BB.first;
	auto max = BB.second;
	glm::vec3 nextPosition = m_eye + m_moveVec * m_deltaTime;
	m_boundingBox.first = nextPosition - m_boxSize.first;
	m_boundingBox.second = nextPosition + m_boxSize.second;
	auto myMin = m_boundingBox.first;
	auto myMax = m_boundingBox.second;
	if (myMin.x > max.x) return false;
	if (myMax.x < min.x) return false;
	if (myMin.z > max.z) return false;
	if (myMax.z < min.z) return false;

	m_cancelMove = true;
	return true;
}

bool FreeCamera::CollisionObject(Static::Object& obj) {
	float offSet{ 3.f };
	auto BB = obj.GetBoundingBox();
	auto min = BB.first;
	auto max = BB.second;
	glm::vec3 nextPosition = m_eye + CameraSpeed * m_moveVec * m_deltaTime;
	m_boundingBox.first = nextPosition - m_boxSize.first;
	m_boundingBox.second = nextPosition + m_boxSize.second;
	auto myMin = m_boundingBox.first;
	auto myMax = m_boundingBox.second;
	if (myMin.x > max.x) return false;
	if (myMax.x < min.x) return false;
	if (myMin.z > max.z) return false;
	if (myMax.z < min.z) return false;

	m_cancelMove = true;
	m_eye -= CameraSpeed *  m_deltaTime *  m_moveVec;
	return true;
}

void FreeCamera::Render() {
	SHADER->GetActivatedShader()->SetUniformMat4("projection", GL_FALSE, &m_projection[0][0]);
	SHADER->GetActivatedShader()->SetUniformMat4("view", GL_FALSE, &m_view[0][0]);
	SHADER->GetActivatedShader()->SetUniformMat4("VP", GL_FALSE, &(m_projection * m_view)[0][0]);
}

void FreeCamera::Update(float deltaTime) {
	m_deltaTime = deltaTime;
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	m_aspect = static_cast<float>(width) / static_cast<float>(height);
	m_moveVec = glm::vec3{ 0.f };

	if (!m_cancelMove) {
		if (Input::GetInstance()->GetKey(GLFW_KEY_W) == KEY_STATE::PRESS) {
			m_moveVec -= m_basisZ;
			m_eye -= m_basisZ * deltaTime * CameraSpeed;
		}

		if (Input::GetInstance()->GetKey(GLFW_KEY_S) == KEY_STATE::PRESS) {
			m_moveVec += m_basisZ;
			m_eye += m_basisZ * deltaTime * CameraSpeed;
		}


		if (Input::GetInstance()->GetKey(GLFW_KEY_A) == KEY_STATE::PRESS) {
			m_moveVec -= m_basisX;
			m_eye -= m_basisX * deltaTime * CameraSpeed;
		}


		if (Input::GetInstance()->GetKey(GLFW_KEY_D) == KEY_STATE::PRESS) {
			m_moveVec += m_basisX;
			m_eye += m_basisX * deltaTime * CameraSpeed;
		}

		if (Input::GetInstance()->GetKey(GLFW_KEY_Q) == KEY_STATE::PRESS) {
			m_moveVec -= m_basisY;
			m_eye -= m_basisY * deltaTime * CameraSpeed;
		}


		if (Input::GetInstance()->GetKey(GLFW_KEY_E) == KEY_STATE::PRESS) {
			m_moveVec += m_basisY;
			m_eye += m_basisY * deltaTime * CameraSpeed;
		}
	}

	m_cancelMove = false;

	float2 deltaMouse = Input::GetInstance()->GetDeltaMouse();

	if (!(deltaMouse.x == 0 and deltaMouse.y == 0)) {
		glm::mat4 cameraRotate{ 1.f };

		cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.y * MOUSE_SENSITIVE), m_basisX);
		cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.x * MOUSE_SENSITIVE), m_basisY);

		m_rotate = cameraRotate;
		m_deltaRotate.y = glm::radians(-deltaMouse.x * MOUSE_SENSITIVE);
		m_deltaRotate.x = glm::radians(-deltaMouse.y * MOUSE_SENSITIVE);

		m_at = glm::normalize(glm::vec3(cameraRotate * glm::vec4(m_at, 1.f)));
		m_basisZ = glm::normalize(-m_at);
		m_basisX = glm::normalize(glm::cross(m_up, m_basisZ));
		m_basisY = glm::cross(m_basisZ, m_basisX);
	}
	else {
		m_deltaRotate = glm::vec3{ };
	}

	m_view = glm::lookAt(m_eye, m_eye + m_at, m_up);
}
