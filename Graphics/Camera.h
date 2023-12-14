#pragma once

namespace Animated {
	class Object;
};
namespace Static {
	class Object;
};

class Camera abstract {
public:
	Camera() = default;
	Camera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT);

	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

protected:
	// To get aspect
	GLFWwindow* m_window{};

	glm::mat4 m_projection{ };
	glm::mat4 m_view{ };

protected:
	glm::vec3 m_eye{};
	glm::vec3 m_at{};
	glm::vec3 m_up{ 0.f,1.f,0.f };

	float m_fovY = 45.f;
	float m_nearZ = 0.1f;
	float m_farZ = 1000.f;

	float m_aspect{};

	glm::vec3 m_basisX{ 1.f,0.f,0.f };
	glm::vec3 m_basisY{ 0.f,1.f,0.f };
	glm::vec3 m_basisZ{ 0.f,0.f,1.f };

	glm::mat4 m_rotate{ 0.f };


public:
	virtual void Render() = 0;
	virtual void Update(float DeltaTime) = 0;


	const glm::mat4* GetViewPtr() { return &m_view; };
	const glm::mat4* GetProjectionPtr() { return &m_projection; };
	const glm::mat4* GetRotateMatPtr() { return &m_rotate; };
	const glm::vec3* GetPositionPtr() { return &m_eye; };
	const glm::vec3* GetBasisZPtr() { return &m_basisZ; };
	glm::vec3 GetViewPoint() const { return m_at; }
	glm::vec3& GetPosition() { return m_eye; }
};



class FreeCamera : public Camera {
public:
	FreeCamera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT);
	~FreeCamera();

private:
	float m_deltaTime{ };
	std::pair<glm::vec3, glm::vec3> m_boxSize{ };
	std::pair<glm::vec3, glm::vec3> m_boundingBox{ };
	glm::vec3 m_moveVec{ };
	bool m_cancelMove{ false };

public:
	bool CollisionObject(Animated::Object& obj);
	bool CollisionObject(Static::Object& obj);

public:
	virtual void Render();
	virtual void Update(float DeltaTime);
};