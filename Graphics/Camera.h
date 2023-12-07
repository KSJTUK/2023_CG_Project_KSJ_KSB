#pragma once


class Camera abstract{
public:
	Camera() = default;
	Camera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT);

	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

protected:

	// To get aspect
	GLFWwindow* m_window{};




protected:
	
	glm::vec3 m_eye{};
	glm::vec3 m_at{};
	glm::vec3 m_up{ 0.f,1.f,0.f };

	float m_fovY = 60.f;
	float m_nearZ = 0.1f;
	float m_farZ = 1000.f;

	float m_aspect{};

	glm::vec3 m_basisX{ 1.f,0.f,0.f };
	glm::vec3 m_basisY{ 0.f,1.f,0.f };
	glm::vec3 m_basisZ{ 0.f,0.f,1.f };



public:
	virtual void Render() = 0;
	virtual void Update(float DeltaTime) = 0;
};



class FreeCamera : public Camera {
public:
	FreeCamera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT);


	virtual void Render();
	virtual void Update(float DeltaTime);

};