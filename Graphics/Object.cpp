#include "pch.h"
#include "Object.h"
#include "Graphics/AnimatedModel.h"
#include "Graphics/StaticModel.h"


namespace Animated {
	Object::Object(){
	}

	Object::Object(std::shared_ptr<Model> model): m_model(model) {
	}

	void Object::SetAnimation(const int& index) {
		m_animationIndex = index;
	}
}

namespace Static {

	Object::Object() {
	}

	Object::Object(std::shared_ptr<Model> model) : m_model(model) {
	}

}