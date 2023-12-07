#pragma once


class Renderer {
public:
	Renderer();
	Renderer(GLFWwindow* window);
	
	~Renderer();

	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;

private:
	std::unique_ptr<class FreeCamera> m_freeCamera{ nullptr };

	GLenum m_renderMode = GL_FILL;

public:
	void Update(float DeltaTime);
	void Render();
};