#include "pch.h"
#include "Zombie.h"

#include "Graphics/AnimatedModel.h"
#include "Util/Math.h"


Animated::Zombie::Zombie(){
}

Animated::Zombie::Zombie(std::shared_ptr<Model> model,const glm::mat4* ViewPointer, const glm::mat4* PerspectivePointer, const glm::vec3* CameraPositionPointer) :
	Object{model}, m_viewptr(ViewPointer), m_perspectiveptr(PerspectivePointer) , m_cameraptr(CameraPositionPointer){

	m_curstate = new Wander();
	m_curstate->Enter(*this);

	m_position = glm::vec3{ 10.f,0.f,10.f };
	m_scale = glm::vec3{ 0.1f,0.1f,0.1f };
	m_rotate = glm::radians( glm::vec3{ -90.f,0.f,0.f } ) ;


}

void Animated::Zombie::Render(){
	m_curstate->Render(*this);
}


void Animated::Zombie::Update(float DeltaTime){

	ZombieState* state_ = m_curstate->Update(DeltaTime,*this);

	if (state_ != nullptr) {
		delete m_curstate;
		m_curstate = state_;
		m_curstate->Enter(*this);
	}


}



// TODO
constexpr glm::vec3 RayDirection_ = glm::vec3{ 0.f,0.f,1.f };
constexpr glm::vec3 RayOrigin_ = glm::vec3{ 0.f };


// NOTE : Optimize Failed... 
bool Animated::Zombie::nRayCasting(){


	// phase 1. direction 
	glm::vec3 TargetDir =  m_position;


	if (glm::dot(TargetDir,RayDirection_ ) <= 0.f) {
		std::cout << "false phase 1" << std::endl;
		return false;
	}
	
	// phase 2. Bounding volume 
	// if code flow reached here, Ray Direction Directed target, so pretend Ray Complete Line is OK( not half Line ) 
	glm::vec3 VolumeCenter{ m_position.x,m_position.y + 5.f, m_position.z };

	if (DistanceRayBetweenPoint(*m_cameraptr, RayDirection_, VolumeCenter) > 10.f) {
		std::cout << "false phase 2" << std::endl;
		return false;
	}


	glm::mat4 WorldMatrix = *m_perspectiveptr * *m_viewptr * m_transform;

	if (m_model->RayCasting(RayOrigin_, RayDirection_, WorldMatrix)) {
		return true;
	}

	std::cout << "false phase 3" << std::endl;
	return false;

}

// State : Wander =====================================================================


void Animated::Wander::Enter(Zombie& zombie){

	zombie.m_animationIndex = 1;

}

Animated::ZombieState* Animated::Wander::Update(float DeltaTime, Zombie& zombie){
	zombie.m_animationCounter += DeltaTime;

	//if (zombie.nRayCasting()) {
	//	std::cout << "Hit" << std::endl;
	//}

	return nullptr;
}

void Animated::Wander::Render(Zombie& zombie){


	zombie.m_transform =
		glm::translate(zombie.m_position) *
		glm::yawPitchRoll(zombie.m_rotate.y, zombie.m_rotate.x, zombie.m_rotate.z) *
		glm::scale(zombie.m_scale);




	zombie.m_model->Render(zombie.m_transform, zombie.m_animationIndex, zombie.m_animationCounter);


}



void Animated::Wander::Exit(Zombie& zombie){

		
}








// State : Chase =====================================================================

void Animated::Chase::Enter(Zombie& zombie)
{
}

Animated::ZombieState* Animated::Chase::Update(float DeltaTime, Zombie& zombie)
{
	return nullptr;
}

void Animated::Chase::Render(Zombie& zombie){
}

void Animated::Chase::Exit(Zombie& zombie){
}
