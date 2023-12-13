#pragma once
namespace Animated { class Object; class Model; class AR15; class Zombie; }
namespace Static { class Model; }


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

	std::unique_ptr<class PointLight> m_testLight{ nullptr };


	GLenum m_renderMode = GL_LINE;



	std::shared_ptr<Animated::Model> ar15_model{ nullptr };
	std::shared_ptr<Static::Model> static_model{ nullptr };



	void CollisionTerrain(class Animated::Object& obj, float offset);


	std::vector<std::shared_ptr<Animated::Object>> m_animatedObjectArr{};


	std::shared_ptr<Animated::Zombie> m_zombie{};


public:
	void Update(float deltaTime);
	void Render();
};