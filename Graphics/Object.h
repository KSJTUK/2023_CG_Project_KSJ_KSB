#pragma once


namespace Animated {
	class Model;

	class Object abstract{
	public:
		Object();
		Object(std::shared_ptr<Model> model);
	protected:
		std::shared_ptr<Model> m_model{ nullptr };

		float m_animationCounter = 0.f;
		int m_animationIndex = 0;


		glm::vec3 m_position{ 0.f };
		glm::vec3 m_scale{ 1.f };
		glm::vec3 m_rotate{ 0.f };

		glm::mat4 m_transform{1.f};

	public:
		glm::vec3& GetPosition() { return m_position; }
		glm::vec3 GetPositionConst() { return m_position; }
		void SetPosition(const glm::vec3& position) { m_position = position; }

		void SetAnimation(const int& index);

	public:
		virtual void Render() = 0;
		virtual void Update(float DeltaTime) = 0;
		virtual bool RayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::mat4& View, const glm::mat4& Projection) const = 0;
	};

}


namespace Static {
	class Model;

	class Object abstract {
	public:
		Object();
		Object(std::shared_ptr<Model> model);

	protected:
		std::shared_ptr<Model> model{ nullptr };


		glm::vec3 m_position{};
		glm::vec3 m_scale{};
		glm::vec3 m_rotate{};

	public:
		
		void SetPosition(const glm::vec3& position) { m_position = position; };

	public:

		virtual void Render() = 0;
		virtual void Update() = 0;
		
	};
}