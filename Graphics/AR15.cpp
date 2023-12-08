#include "pch.h"
#include "AR15.h"

#include "Util/Input.h"
#include "Graphics/AnimatedModel.h"


Animated::AR15::AR15(){
}

Animated::AR15::AR15(std::shared_ptr<Model> model) : Object{ model } {
}

void Animated::AR15::Render(){
	m_model->Render();
}

void Animated::AR15::Update(float DeltaTime){

}
