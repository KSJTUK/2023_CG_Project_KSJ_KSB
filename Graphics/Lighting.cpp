#include "pch.h"
#include "Lighting.h"
#include "Graphics/Shader.h"

PointLight::PointLight() { }

PointLight::~PointLight() { }

void PointLight::SetPosition(const glm::vec3& position) {
	m_lightPosition = position;
}

void PointLight::ChangeAmbient(const glm::vec3& ambient) {
	m_ambient = ambient;
}

void PointLight::ChangeLightColor(const glm::vec3& color) {
	m_lightColor = color;
}

void PointLight::SetLinearQuadrant(float linear, float quadrant) {
	m_linear = linear;
	m_quadratic = quadrant;
}

void PointLight::Render() {
	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.position", &m_lightPosition[0]);

	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.ambient", &m_ambient[0]);
	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.diffuse", &m_lightColor[0]);
	SHADER->GetActivatedShader()->SetUniformVec3("pointLight.specular", &m_specular[0]);

	SHADER->GetActivatedShader()->SetUniformFloat("pointLight.constant", m_constant);
	SHADER->GetActivatedShader()->SetUniformFloat("pointLight.linear", m_linear);
	SHADER->GetActivatedShader()->SetUniformFloat("pointLight.quadratic", m_quadratic);
}

SpotLight::SpotLight() { }

SpotLight::~SpotLight() { }
