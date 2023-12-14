#include "pch.h"
#include "PineTree.h"
#include "StaticModel.h"


Static::PineTree::PineTree(){
}

Static::PineTree::PineTree(std::shared_ptr<Model> model,const glm::vec3& Position) : Object{ model } {
	m_position = Position;
	m_scale = glm::vec3{ 0.5f,0.5f,0.5f };
}

void Static::PineTree::Render(){

	m_transform = glm::translate(m_position) *
		glm::yawPitchRoll(m_rotate.y, m_rotate.x, m_rotate.z) *
		glm::scale(m_scale);

	m_model->Render(m_transform);
}

void Static::PineTree::Update(float DeltaTime)
{
}
