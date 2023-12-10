#include "pch.h"
#include "AR15.h"

#include "Util/Input.h"
#include "Graphics/AnimatedModel.h"


Animated::AR15::AR15(){
	m_scale = glm::vec3{ 0.1f,0.1f,0.1f };
	m_rotate = glm::radians(glm::vec3{ -90.f,0.f,0.f });
}

Animated::AR15::AR15(std::shared_ptr<Model> model) : Object{ model } {
	m_scale = glm::vec3{ 0.5f,0.5f,0.5f };
	m_rotate = glm::radians(glm::vec3{ -90.f,0.f,0.f });
}

void Animated::AR15::Render(){


	m_transform =
		glm::translate(m_position) *
		glm::yawPitchRoll(m_rotate.y, m_rotate.x, m_rotate.z) *
		glm::scale(m_scale);


	m_model->Render(m_transform,m_animationIndex,m_animationCounter);
}

void Animated::AR15::Update(float DeltaTime){
	m_animationCounter += DeltaTime;
}

bool Animated::AR15::RayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::mat4& View, const glm::mat4& Projection) const{
	
	glm::mat4 WorldMatrix = Projection * View * m_transform;

	if (m_model->RayCasting(RayOrigin, RayDirection,WorldMatrix)) {
		return true;
	}
	return false;
}


void Animated::AR15::SetAnimationIndex(int val) {
	m_animationIndex = val;
}

void Animated::AR15::SetPosition(const glm::vec3& pos){
	m_position = pos;

}
