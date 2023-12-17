#include "pch.h"
#include "Zombie.h"

#include "Graphics/AnimatedModel.h"
#include "Graphics/Shader.h"
#include "Util/Input.h"
#include "Util/Math.h"

#include "Sound.h"

Animated::Zombie::Zombie(){
}

Animated::Zombie::Zombie(std::shared_ptr<Model> model,const glm::mat4* ViewPointer, const glm::mat4* PerspectivePointer, const glm::vec3* CameraPositionPointer, const glm::vec3* CameraBasisZ) :
	Object{model}, m_viewptr(ViewPointer), m_perspectiveptr(PerspectivePointer) , m_cameraPosition(CameraPositionPointer), m_cameraBasisz(CameraBasisZ) {

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
		m_curstate->Exit(*this);
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
	// phase 2. Bounding volume 
	// if code flow reached here, Ray Direction Directed target, so pretend Ray Complete Line is OK( not half Line ) 
	glm::vec3 VolumeCenter{ m_position.x,m_position.y + 1.f, m_position.z };

	float d = DistanceRayBetweenPoint(*m_cameraPosition, *(m_cameraBasisz), VolumeCenter);
	if (d > 50.f) {
		return false;
	}


	glm::mat4 WorldMatrix = (*m_perspectiveptr) * (*m_viewptr) * m_transform;

	if (m_model->RayCasting(RayOrigin_, RayDirection_, WorldMatrix)) {
		return true;
	}

	//std::cout << "false phase 3" << std::endl;
	return false;

}

// State : Wander =====================================================================
void Animated::Wander::Enter(Zombie& zombie){
	zombie.m_animationIndex = 2;
	m_timeCount = m_randomMoveTime * 0.5f;
}

Animated::ZombieState* Animated::Wander::Update(float deltaTime, Zombie& zombie){
	m_timeCount += deltaTime;
	zombie.m_animationCounter += deltaTime;

	zombie.m_position += m_moveSpeed * deltaTime;
	zombie.m_position = glm::clamp(zombie.m_position, -1000.f, 1000.f);
	if (m_timeCount > m_randomMoveTime) {
		m_timeCount = 0.f;
		m_moveSpeed = glm::linearRand(glm::vec3{ m_minSpeed, 0.f, m_minSpeed }, glm::vec3{ m_maxSpeed, 0.f, m_maxSpeed });
		zombie.m_rotate.y += glm::orientedAngle(m_prevDirection, glm::normalize(m_moveSpeed), glm::vec3{ 0.f, 1.f, 0.f });
		m_prevDirection = glm::normalize(m_moveSpeed);
	}
	
	if (zombie.nRayCasting() and !INPUT->m_notfired) { 
		zombie.m_damaged = true;

		return new Hited();
	}


	if (glm::linearRand(0.f, 1.f) > 0.00001f) {
		//Sound::GetInstance()->Play(std::string("zombie_idle"),1);
	}




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

void Animated::Hited::Enter(Zombie& zombie) {

	Sound::GetInstance()->Play(std::string("zombie_hit"),2);

}

Animated::ZombieState* Animated::Hited::Update(float DeltaTime, Zombie& zombie) {
	if (!zombie.m_damaged) {

		return new Wander();
	}

	return nullptr;
}

void Animated::Hited::Render(Zombie& zombie){

	SHADER->UseProgram(ShaderType::AnimatedShader);
	SHADER->GetActivatedShader()->SetUniformInt("damaged", 1);
	

	zombie.m_transform =
		glm::translate(zombie.m_position) *
		glm::yawPitchRoll(zombie.m_rotate.y, zombie.m_rotate.x, zombie.m_rotate.z) *
		glm::scale(zombie.m_scale);

	zombie.m_model->Render(zombie.m_transform, zombie.m_animationIndex, zombie.m_animationCounter);
	zombie.m_damaged = false;



	SHADER->GetActivatedShader()->SetUniformInt("damaged",0);
	SHADER->UnuseProgram();


}

void Animated::Hited::Exit(Zombie& zombie){
}
