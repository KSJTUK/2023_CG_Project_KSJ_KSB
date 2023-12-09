#pragma once
namespace Animated { class Object; class Model; class AR15; }

class Renderer {
public:
	Renderer();
	Renderer(GLFWwindow* window);
	
	~Renderer();

	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;

private:
	std::unique_ptr<class FreeCamera> m_freeCamera{ nullptr };
	std::unique_ptr<class SkyBox> m_background{ nullptr };
	std::unique_ptr<class Terrain> m_testTerrain{ nullptr };

	std::unique_ptr<class Model> m_testModel{ nullptr };

	GLenum m_renderMode = GL_LINE;



	std::shared_ptr<Animated::Model> ar15_model{ nullptr };

	void CollisionTerrain(class Animated::Object& obj, float offset);


	std::vector<std::shared_ptr<Animated::Object>> m_animatedObjectArr{};



public:
	void Update(float deltaTime);
	void Render();
};