#pragma once

constexpr float DIRECTION_MINANGLE = 0.f;
constexpr float DIRECTION_MAXANGLE = 180.f;
constexpr float DAYTIME_SPEED = 0.001f;
constexpr float DAY_LIGHT_MAX_AMBIENT = 0.5f;
constexpr glm::vec3 ROTATE_AXIS_Z = { 0.f, 0.f, 1.f };

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

class DirectionLight {
public:
	DirectionLight();
	~DirectionLight();

private:
	glm::vec3 m_ambient{ 0.f };
	glm::vec3 m_lightColor{ 1.f };
	glm::vec3 m_lightDirection{ -1.f, 0.f, 0.f };
	float m_directionAngle{ 0.f };
	float m_angleDir{ 1.f };

public:
	void DayUpdate(float deltaTime);
	glm::vec3& GetDirLightAmbient() { return m_ambient; }
	glm::vec3& GetDirLightColor() { return m_lightColor; }

	void ChangeDirection(const glm::vec3& direction);

	void ChangeAmbient(const glm::vec3& ambient);
	void ChangeLightColor(const glm::vec3& color);

public:
	void Render();
};