#include "pch.h"
#include "SkyBox.h"
#include "Graphics/GraphicBuffers.h"
#include "Graphics/Shader.h"
#include "Util/TextureComponent.h"

SkyBox::SkyBox() {
	SHADER->UseProgram(ShaderType::BackgroundShader);

	std::vector<glm::vec3> temp{
		glm::vec3{ -1.0f,  1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{  1.0f, -1.0f, -1.0f },
		glm::vec3{  1.0f, -1.0f, -1.0f },
		glm::vec3{  1.0f,  1.0f, -1.0f },
		glm::vec3{ -1.0f,  1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f,  1.0f, -1.0f },
		glm::vec3{ -1.0f,  1.0f, -1.0f },
		glm::vec3{ -1.0f,  1.0f,  1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{  1.0f, -1.0f, -1.0f },
		glm::vec3{  1.0f, -1.0f,  1.0f },
		glm::vec3{  1.0f,  1.0f,  1.0f },
		glm::vec3{  1.0f,  1.0f,  1.0f },
		glm::vec3{  1.0f,  1.0f, -1.0f },
		glm::vec3{  1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{ -1.0f,  1.0f,  1.0f },
		glm::vec3{  1.0f,  1.0f,  1.0f },
		glm::vec3{  1.0f,  1.0f,  1.0f },
		glm::vec3{  1.0f, -1.0f,  1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{ -1.0f,  1.0f, -1.0f },
		glm::vec3{  1.0f,  1.0f, -1.0f },
		glm::vec3{  1.0f,  1.0f,  1.0f },
		glm::vec3{  1.0f,  1.0f,  1.0f },
		glm::vec3{ -1.0f,  1.0f,  1.0f },
		glm::vec3{ -1.0f,  1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{  1.0f, -1.0f, -1.0f },
		glm::vec3{  1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{  1.0f, -1.0f,  1.0f },
	};
	m_verticies = temp;

	m_textures = std::make_unique<CubeMapTexture>();
	const std::string dir{ "./textures/" };

	m_textures->LoadCubeMapTexture(dir + "interstellar_ft.tga", dir + "interstellar_bk.tga", dir + "interstellar_up.tga",
		dir + "interstellar_dn.tga", dir + "interstellar_rt.tga", dir + "interstellar_lf.tga", false);

	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, uint32(m_verticies.size()) * sizeof(glm::vec3), &m_verticies[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glDeleteBuffers(1, &m_VBO);
	glBindVertexArray(0);

	SHADER->UnuseProgram();
}

SkyBox::~SkyBox() { }

void SkyBox::Render() {
	SHADER->UseProgram(ShaderType::BackgroundShader);
	SHADER->GetActivatedShader()->SetUniformInt("cubeMapTexture", 0);


	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(m_VAO);
	m_textures->BindCubeMap(0);

	glDrawArrays(GL_TRIANGLES, 0, uint32(m_verticies.size()));
	m_textures->UnBindCubeMap();

	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	SHADER->UnuseProgram();
}
