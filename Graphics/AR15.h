#pragma once
#include "Object.h"

namespace Animated {
	class AR15 final: public Object {
	public:
		AR15();
		AR15(std::shared_ptr<Model> model);

	public:

		virtual void Render() override;
		virtual void Update(float DeltaTime) override;


	public:

		void SetAnimationIndex(int val);
		void SetPosition(const glm::vec3& pos);

	};



}

