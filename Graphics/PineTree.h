#pragma once
#include "Object.h"


namespace Static {
	

	class PineTree : public Object{
	public:
		PineTree();
		PineTree(std::shared_ptr<Model> model,const glm::vec3& Position);
	public:

		virtual void Render() override;
		virtual void Update(float DeltaTime) override;

	};

}

