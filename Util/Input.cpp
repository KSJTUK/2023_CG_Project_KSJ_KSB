#include "pch.h"
#include "Input.h"
#include "Sound.h"


Input* Input::m_instance = nullptr;

Input::Input() { }

Input::~Input() {
	if (m_keyboardState) {
		delete[] m_keyboardState;
	}
	m_keyboardState = nullptr;
}

void Input::Init(GLFWwindow* window) {

	m_instance->m_window = window;


	m_instance->m_keyboardState = new KEY_STATE[256];

	for (auto i = 0; i < 256; ++i) {
		m_instance->m_keyboardState[i] = KEY_STATE::NONE;
	}

	double tx, ty;
	glfwGetCursorPos(m_instance->m_window, &tx, &ty);
	m_instance->m_prevMouse = float2{ static_cast<float>(tx),static_cast<float>(ty) };
}

Input* Input::GetInstance() {
	if (m_instance == nullptr) {
		m_instance = new Input();
	}

	return m_instance;
}

void Input::Update() {
	BYTE temp[256]{ };
	if (!GetKeyboardState(temp)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	if (temp[VK_ESCAPE] & 0x80) {
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	for (auto i = 0; i < 256; ++i) {
		if (temp[i] & 0x80) {
			if (m_keyboardState[i] == KEY_STATE::NONE or m_keyboardState[i] == KEY_STATE::RELEASE) {
				m_keyboardState[i] = KEY_STATE::DOWN;
			}
			else if (m_keyboardState[i] == KEY_STATE::DOWN) {
				m_keyboardState[i] = KEY_STATE::PRESS;
			}
		}
		else {
			if (m_keyboardState[i] == KEY_STATE::PRESS or m_keyboardState[i] == KEY_STATE::DOWN) {
				m_keyboardState[i] = KEY_STATE::RELEASE;
			}
			else if (m_keyboardState[i] == KEY_STATE::RELEASE) {
				m_keyboardState[i] = KEY_STATE::NONE;
			}
		}
	}

	double tx, ty;
	glfwGetCursorPos(m_window, &tx, &ty);

	m_deltaMouse.x = static_cast<float>(tx) - m_prevMouse.x;
	m_deltaMouse.y = static_cast<float>(ty) - m_prevMouse.y;

	m_prevMouse.x = static_cast<float>(tx);
	m_prevMouse.y = static_cast<float>(ty);


	
	
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS and m_notfired) {
		Sound::GetInstance()->Play(std::string("Gun"),0);
		m_notfired = false;

	}

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		m_notfired = true;
	}



}

const KEY_STATE Input::GetKey(int key) const {
	return m_keyboardState[key];
}
