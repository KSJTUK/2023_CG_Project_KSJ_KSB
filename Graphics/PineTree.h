#pragma once
#include "Object.h"

namespace Static {
	class PineTree : public Object{
	public:
		PineTree();
		PineTree(std::shared_ptr<Model> model,const glm::vec3& position);
	public:

		virtual void Render() override;
		virtual void Update(float deltaTime) override;

	};

}

