#include "pch.h"
#include "PineTree.h"
#include "StaticModel.h"


Static::PineTree::PineTree(){
}

Static::PineTree::PineTree(std::shared_ptr<Model> model,const glm::vec3& Position) : Object{ model } {
	m_position = Position;
	m_scale = glm::vec3{ glm::linearRand(0.3f, 0.6f) };
	glm::vec2 xWidth{ -15.f, 15.f };
	glm::vec2 zWidth{ -15.f, 15.f };
	glm::vec2 height{ 0.f, 900.f };

	m_transform = glm::translate(m_position) *
		glm::yawPitchRoll(m_rotate.y, m_rotate.x, m_rotate.z) *
		glm::scale(m_scale);

	m_boxSize = {
		glm::vec3{ xWidth[0], height[0], zWidth[0] },
		glm::vec3{ xWidth[1], height[1], zWidth[1] }
	};


	m_boundingBox.first = m_transform * glm::vec4{ m_boxSize.first, 1.f };
	m_boundingBox.second = m_transform * glm::vec4{ m_boxSize.second, 1.f };
}

void Static::PineTree::Render(){
	m_transform = glm::translate(m_position) *
		glm::yawPitchRoll(m_rotate.y, m_rotate.x, m_rotate.z) *
		glm::scale(m_scale);

	m_boundingBox.first = m_transform * glm::vec4{ m_boxSize.first, 1.f };
	m_boundingBox.second = m_transform * glm::vec4{ m_boxSize.second, 1.f };

	m_model->Render(m_transform);
}

void Static::PineTree::Update(float DeltaTime)
{
}
