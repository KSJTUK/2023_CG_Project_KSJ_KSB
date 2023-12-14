#include "pch.h"
#include "AR15.h"

#include "Util/Input.h"
#include "Graphics/AnimatedModel.h"




Animated::AR15::AR15(std::shared_ptr<Model> model,const glm::vec3* CameraPosition,const glm::mat4* CameraRotation) : Object{model}, mp_cameraPosition(CameraPosition), mp_cameraRotation(CameraRotation)  {

	m_scale = glm::vec3{ 0.01f,0.01f,0.01f };
	m_rotate = glm::radians(glm::vec3{ -90.f,0.f,0.f });
	m_position = *mp_cameraPosition;
	m_position.y += 11.f;
	m_position.x -= 2.f;

	m_curstate = new Idle();
	m_curstate->Enter(*this);

}

void Animated::AR15::Render(){


	m_curstate->Render(*this);
}

void Animated::AR15::Update(float DeltaTime){

	AR15State* state_ = m_curstate->Update(DeltaTime,*this);

	if (state_ != nullptr) {
		m_curstate->Exit(*this);
		delete m_curstate;
		m_curstate = state_;
		m_curstate->Enter(*this);
	}
}





const glm::vec3 ar15_layout = glm::vec3{ 2.f,0.f,-1.f };

void Animated::Idle::Enter(AR15& ar15){


	ar15.m_animationIndex = 4;
	ar15.m_animationCounter = 0.f;


}

Animated::AR15State* Animated::Idle::Update(float DeltaTime, AR15& ar15){

	ar15.m_animationCounter += DeltaTime ;


	

	return nullptr;

}

void Animated::Idle::Render(AR15& ar15){



	ar15.m_transform = (*ar15.mp_cameraRotation) *
		glm::translate(ar15.m_position) *
		glm::yawPitchRoll(ar15.m_rotate.y, ar15.m_rotate.x, ar15.m_rotate.z) *
		glm::scale(ar15.m_scale);



	ar15.m_model->Render(ar15.m_transform, ar15.m_animationIndex, ar15.m_animationCounter);



}

void Animated::Idle::Exit(AR15& ar15){


}







void Animated::Fire::Enter(AR15& ar15)
{
}

Animated::AR15State* Animated::Fire::Update(float DeltaTime, AR15& ar15)
{
	return nullptr;
}

void Animated::Fire::Render(AR15& ar15)
{
}

void Animated::Fire::Exit(AR15& ar15)
{
}










void Animated::Reload::Enter(AR15& ar15)
{
}

Animated::AR15State* Animated::Reload::Update(float DeltaTime, AR15& ar15)
{
	return nullptr;
}

void Animated::Reload::Render(AR15& ar15)
{
}

void Animated::Reload::Exit(AR15& ar15)
{
}
