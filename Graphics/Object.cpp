#include "pch.h"
#include "Object.h"
#include "Graphics/AnimatedModel.h"

namespace Animated {
	Object::Object(){
	}

	Object::Object(std::shared_ptr<Model> model): m_model(model) {
	}
}

namespace Static {



}