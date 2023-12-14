#pragma once
#include "Object.h"

namespace Animated {

	class AR15;

	class AR15State {
	public:
		virtual void Enter(AR15& ar15) = 0;
		virtual AR15State* Update(float DeltaTime, AR15& ar15) = 0;
		virtual void Render(AR15& ar15) = 0;
		virtual void Exit(AR15& ar15) = 0;
	};


	class Idle : public AR15State {
	public:
		virtual void Enter(AR15& ar15) override ;
		virtual AR15State* Update(float DeltaTime, AR15& ar15) override;
		virtual void Render(AR15& ar15) override;
		virtual void Exit(AR15& ar15) override;
	};



	class Fire : public AR15State {
	public:
		virtual void Enter(AR15& ar15) override;
		virtual AR15State* Update(float DeltaTime, AR15& ar15) override;
		virtual void Render(AR15& ar15) override;
		virtual void Exit(AR15& ar15) override;
	};


	class Reload : public AR15State {
	public:
		virtual void Enter(AR15& ar15) override;
		virtual AR15State* Update(float DeltaTime, AR15& ar15) override;
		virtual void Render(AR15& ar15) override;
		virtual void Exit(AR15& ar15) override;
	};







	class AR15 final: public Object {
		friend Idle;
		friend Fire;
		friend Reload;

	public:
		
		AR15(std::shared_ptr<Model> model,const glm::vec3* CameraPosition,const glm::mat4* CameraRotation);

	public:
	
		virtual void Render() override;
		virtual void Update(float DeltaTime) override;


	private:

		AR15State* m_curstate{ nullptr };


		const glm::vec3* mp_cameraPosition{ nullptr };
		const glm::mat4* mp_cameraRotation{ nullptr };


	};



}
 
