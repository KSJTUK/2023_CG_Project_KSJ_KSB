#include "pch.h"
#include "Zombie.h"

#include "Graphics/AnimatedModel.h"



Animated::Zombie::Zombie(){
}

Animated::Zombie::Zombie(std::shared_ptr<Model> model) : Object{model} {
}

void Animated::Zombie::Render(){
	m_transform =
		glm::translate(m_position) *
		glm::yawPitchRoll(m_rotate.y, m_rotate.x, m_rotate.z) *
		glm::scale(m_scale);


	m_model->Render(m_transform, m_animationIndex, m_animationCounter);
}

void Animated::Zombie::Update(float DeltaTime){



}

bool Animated::Zombie::RayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::mat4 View, const glm::mat4& Projection){


	// phase 1. direction 
	glm::vec3 TargetDir = RayOrigin - m_position;

	if (glm::dot(TargetDir, RayDirection) <= 0.f) {
		std::cout << "false phase 1" << std::endl;
		return false;
	}


	// phase 2. Bounding volume 












	glm::mat4 WorldMatrix = Projection * View * m_transform;

	if (m_model->RayCasting(RayOrigin, RayDirection, WorldMatrix)) {
		return true;
	}
	return false;
}
