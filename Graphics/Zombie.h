#pragma once
#include "Object.h"


namespace Animated {
	/*
	StateMachine 

	From 2D Game Programming...

	enter = Constructor of each state 
	exit = Desstructor of each state 

	Update = do 
	Render = draw 
	
	
	*/
	class Zombie;


	class ZombieState {
	public:
		virtual void Enter(Zombie& zombie) = 0;
		virtual ZombieState* Update(float DeltaTime, Zombie& zombie) = 0;
		virtual void Render(Zombie& zombie) = 0;
		virtual void Exit(Zombie& zombie) = 0;
		
	};

	class Wander final : public ZombieState {
	public:
		virtual void Enter(Zombie& zombie) override;
		virtual ZombieState* Update(float DeltaTime, Zombie& zombie) override;
		virtual void Render(Zombie& zombie) override;
		virtual void Exit(Zombie& zombie) override;

	};

	class Chase final : public ZombieState {
	public:
		virtual void Enter(Zombie& zombie) override;
		virtual ZombieState* Update(float DeltaTime, Zombie& zombie) override;
		virtual void Render(Zombie& zombie) override;
		virtual void Exit(Zombie& zombie) override;
	};
	




	class Zombie final : public Object {

		// State Friends 
		friend Wander;
		friend Chase;

	public:

		Zombie();
		Zombie(std::shared_ptr<Model> model,const glm::mat4* ViewPointer,const glm::mat4* PerspectivePointer,const glm::vec3* CameraPositionPointer);
	public:

		virtual void Render() override;
		virtual void Update(float DeltaTime) override;
	



	private:

		ZombieState* m_curstate{ nullptr };

		const glm::mat4* m_viewptr{ nullptr };
		const glm::mat4* m_perspectiveptr{ nullptr };
		const glm::vec3* m_cameraptr{ nullptr };


		bool nRayCasting();


	};





}
