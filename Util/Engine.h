#pragma once
#include "Util/Timer.h"


class Engine {
public:
	Engine();
	~Engine();

private:
	// 윈도우 정보를 담을 구조체
	WindowInformation m_windowInfo{ };

	// 스왑버퍼의 스왑 간격을 조정할 변수 기본은 0 느리게 하려면 큰값을 주면 됨
	int32 m_swapInterver{ };



	std::unique_ptr<Timer> m_timer{};


public:
	void Init();
	void Update();
	void LateUpdate();
	void Render();

	void Loop();
};