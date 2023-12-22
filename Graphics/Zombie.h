#pragma once
#include "Object.h"


namespace Animated {
	class Zombie;

	class ZombieState {
	public:
		virtual void Enter(Zombie& zombie) = 0;
		virtual ZombieState* Update(float deltaTime, Zombie& zombie) = 0;
		virtual void Render(Zombie& zombie) = 0;
		virtual void Exit(Zombie& zombie) = 0;
	};

	class Wander final : public ZombieState {
	private:
		float m_minSpeed{ -5.f };
		float m_maxSpeed{ 5.f };
		glm::vec3 m_moveSpeed{ };
		glm::vec3 m_prevDirection{ 0.f, 0.f, 1.f };
		float m_randomMoveTime{ glm::linearRand(3.f, 10.f) };
		float m_timeCount{ };

	public:
		virtual void Enter(Zombie& zombie) override;
		virtual ZombieState* Update(float deltaTime, Zombie& zombie) override;
		virtual void Render(Zombie& zombie) override;
		virtual void Exit(Zombie& zombie) override;

	};

	class Hited final : public ZombieState {
	public:
		virtual void Enter(Zombie& zombie) override;
		virtual ZombieState* Update(float deltaTime, Zombie& zombie) override;
		virtual void Render(Zombie& zombie) override;
		virtual void Exit(Zombie& zombie) override;
	};

	class Zombie final : public Object {
		// State Friends 
		friend Wander;
		friend Hited;

	public:
		Zombie();
		Zombie(std::shared_ptr<Model> model,const glm::mat4* viewPointer,const glm::mat4* perspectivePointer,const glm::vec3* cameraPositionPointer, const glm::vec3* cameraBasisZ);

	public:
		virtual void Render() override;
		virtual void Update(float deltaTime) override;
	
	private:
		ZombieState* m_curstate{ nullptr };

		const glm::mat4* m_viewptr{ nullptr };
		const glm::mat4* m_perspectiveptr{ nullptr };
		const glm::vec3* m_cameraPosition{ nullptr };
		const glm::vec3* m_cameraBasisz{ nullptr };

		bool nRayCasting();
		bool m_damaged = false;
	};
}