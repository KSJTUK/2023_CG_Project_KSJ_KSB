#include "pch.h"
#include "Timer.h"


Timer::Timer()
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
	if (m_frequency == 0) {
		std::cerr << "Timer Failure : This System does not provide high performance timer" << std::endl;
		exit(EXIT_FAILURE);
	}

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_prev));

}



void Timer::Update() {
	int64 curr;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));

	m_deltaTime = (curr - m_prev) / static_cast<float>(m_frequency);

	m_elapsed += m_deltaTime;


	m_frameCount++;



	if (m_elapsed >= 1.f) {
		m_fps = m_frameCount;
		m_frameCount = 0;
		m_elapsed = 0.f;
	}

	m_prev = curr;

}
