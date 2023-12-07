#pragma once
#include "Graphics/Camera.h"


class Renderer {
public:
	Renderer() = default;
	Renderer(GLFWwindow* window);
	
	~Renderer() = default;

	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;


private:

	
	std::unique_ptr<FreeCamera> m_freeCamera{ nullptr };

	GLenum m_renderMode = GL_FILL;


public:

	void Render();
	void Update(float DeltaTime);
};