#pragma once

#pragma once
class Timer
{
public:
	Timer();
	Timer(const Timer& other) = delete;

	~Timer() = default;

	void Update();

	float GetDeltaTime() { return m_deltaTime; };
	int GetFps() { return m_fps; };

private:

	int64 m_frequency{  };
	int64 m_prev{  };

	float m_deltaTime{};
	float m_elapsed{};

	int m_fps{};
	int m_frameCount{};

};



