#pragma once


namespace Animated {
	class Model;

	class Object abstract{
	public:
		Object();
		Object(std::shared_ptr<Model> model);
	protected:
		std::shared_ptr<Model> m_model{ nullptr };
	public:
		virtual void Render() = 0;
		virtual void Update(float DeltaTime) = 0;
	};

}