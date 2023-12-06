#include "pch.h"
#include "Shader.h"
Shader* Shader::m_instance = nullptr;




void Shader::Initialize(){


}




Shader* Shader::GetInstance()
{
	if (m_instance == nullptr) {
		m_instance = new Shader{};
	}

	return m_instance;
}




void Shader::UseProgram(ShaderType type){
	m_shaderList[static_cast<int>(type)]->UseProgram();
	m_activatedShader = m_shaderList[static_cast<int>(type)];
}

void Shader::UnuseProgram() {
	glUseProgram(INVALID);
}

void ShaderComponent::SetUniformMat4(std::string valueName, GLenum transpose, const float* val)
{
}

void ShaderComponent::SetUniformMat3(std::string valueName, GLenum transpose, const float* val)
{
}

void ShaderComponent::SetUniformVec4(std::string valueName, const float v1, const float v2, const float v3, const float v4)
{
}

void ShaderComponent::SetUniformVec3(std::string valueName, const float v1, const float v2, const float v3)
{
}

void ShaderComponent::SetUniformVec2(std::string valueName, const float v1, const float v2)
{
}

void ShaderComponent::SetUniformFloat(std::string valueName, const float value)
{
}

void ShaderComponent::SetUniformInt(std::string valueName, const int value)
{
}

int ShaderComponent::GetUniformLocation(std::string valueName)
{
	int result = glGetUniformLocation(m_id, valueName.c_str());



#ifdef DEBUG
	assert(result != -1);
#else // Release
	if (result == -1) {
		std::cerr << "Error in\nFile : " << __FILE__ << "\n" << "Line : " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
#endif

	return 0;
}
