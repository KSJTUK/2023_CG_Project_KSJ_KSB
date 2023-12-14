#pragma once

class PointLight {
public:
	PointLight();
	~PointLight();

private:
	glm::vec3 m_ambient{ 0.2f };
	glm::vec3 m_lightColor{ 1.f };

	glm::vec3 m_lightPosition{ };

	float m_linear{ 0.027f };
	float m_quadratic{ 0.0014f };
	const float m_constant{ 1.f };

public:
	void SetPosition(const glm::vec3& position);
	void SetOptions(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& color, float linear, float quadrant);

	void ChangeAmbient(const glm::vec3& ambient);
	void ChangeLightColor(const glm::vec3& color);

	void SetLinearQuadrant(float linear, float quadrant);

public:
	void Render();
};

class DirLight {
public:
	DirLight();
	~DirLight();

private:
	glm::vec3 m_ambient{ 0.f };
	glm::vec3 m_lightColor{ 1.f };
	glm::vec3 m_lightDirection{ };

public:
	void ChangeDirection(const glm::vec3& direction);

	void ChangeAmbient(const glm::vec3& ambient);
	void ChangeLightColor(const glm::vec3& color);

public:
	void Render();
};

class SpotLight {
public:
	SpotLight();
	~SpotLight();

private:
	glm::vec3 m_ambient{ 0.f };
	glm::vec3 m_lightColor{ 0.7f };

	glm::vec3 m_lightPosition{ };
	glm::vec3 m_lightDirection{ };

	float m_cutOff{ std::cosf(glm::radians(12.5f)) };
	float m_outterCutOff{ std::cosf(glm::radians(17.5f)) };

	float m_linear{ 0.27f };
	float m_quadratic{ 0.014f };
	const float m_constant{ 1.f };

public:
	void SetPosition(const glm::vec3& position);
	void SetDirection(const glm::vec3& direction);

	void ChangeAmbient(const glm::vec3& ambient);
	void ChangeLightColor(const glm::vec3& color);

	void SetLinearQuadrant(float linear, float quadrant);
	void SetOutterCutOff(float cutOff, float outterCutoff);

public:
	void Render();
};