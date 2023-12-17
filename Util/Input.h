#pragma once

enum class KEY_STATE {
	/// <summary>
	///  nothing state 
	/// </summary>
	NONE,

	/// <summary>
	///  pressed on time 
	/// </summary>
	DOWN,

	/// <summary>
	///  pressing during time 
	/// </summary>
	PRESS,

	/// <summary>
	///  released on time  
	/// </summary>
	RELEASE
};

class Input {
private:
	Input();
	~Input();

	Input(const Input& other) = delete;
	Input& operator=(const Input& other) = delete;

private:
	static Input* m_instance;

public:
	static Input* GetInstance();

private:
	GLFWwindow* m_window{ nullptr };

	KEY_STATE* m_keyboardState{ nullptr };

	float2 m_prevMouse{ };
	float2 m_deltaMouse{ };

public:
	const KEY_STATE GetKey(int key) const;
	const float2 GetDeltaMouse() const { return m_deltaMouse; };

public:
	void Init(GLFWwindow* window);
	void Update();

	bool m_notfired = true;
private:


};

#define INPUT Input::GetInstance()

