#include "pch.h"
#include "AR15.h"

#include "Util/Input.h"
#include "Graphics/AnimatedModel.h"


Animated::AR15::AR15(){
	m_scale = glm::vec3{ 0.1f,0.1f,0.1f };
	m_rotate = glm::radians(glm::vec3{ -90.f,0.f,0.f });
}

Animated::AR15::AR15(std::shared_ptr<Model> model) : Object{ model } {
	m_scale = glm::vec3{ 0.1f,0.1f,0.1f };
	m_rotate = glm::radians(glm::vec3{ -90.f,0.f,0.f });
}

void Animated::AR15::Render(){

	glm::mat4 WorldMatrix{ 
		glm::translate(m_position) * 
		glm::yawPitchRoll(m_rotate.y,m_rotate.x,m_rotate.z) *
		glm::scale(m_scale)
	};

	m_model->Render(WorldMatrix,m_animationIndex,m_animationCounter);
}

void Animated::AR15::Update(float DeltaTime){
	m_animationCounter += DeltaTime;
}


void Animated::AR15::SetAnimationIndex(int val) {
	m_animationIndex = val;
}

void Animated::AR15::SetPosition(const glm::vec3& pos){
	m_position = pos;

}
