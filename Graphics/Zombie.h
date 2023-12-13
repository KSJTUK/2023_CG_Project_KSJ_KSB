#pragma once
#include "Object.h"


namespace Animated {
	class Zombie final : public Object {
	public:

		Zombie();
		Zombie(std::shared_ptr<Model> model);
	public:

		virtual void Render() override;
		virtual void Update(float DeltaTime) override;
	

		bool RayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::mat4 View, const glm::mat4& Projection);

	};






}

