#include "pch.h"
#include "Shader.h"
Shader* Shader::m_instance = nullptr;




void Shader::Initialize(){


}




Shader* Shader::GetInstance(){
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


int ShaderComponent::GetUniformLocation(std::string valueName){
	int result = glGetUniformLocation(m_id, valueName.c_str());
#ifdef _DEBUG
	assert(result != -1);
#else // Release
	if (result == -1) {
		std::cerr << "Error in\nFile : " << __FILE__ << "\n" << "Line : " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
#endif

	return result;
}

ShaderComponent::ShaderComponent(std::string VertexShaderPath, std::string GeometryShaderPath, std::string TesselationEvaluationShaderPath, std::string TesselationControlShaderPath, std::string FragmentShaderPath)
{
}

void ShaderComponent::SetUniformMat4(std::string valueName, GLenum transpose, const float* val){

	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniformMatrix4fv(Loc, 1, transpose, val);
		m_uniformLocationDict.insert(std::make_pair(valueName, Loc));
	}
	else {
		glUniformMatrix4fv(iter->second, 1, transpose, val);
	}
}

void ShaderComponent::SetUniformMat3(std::string valueName, GLenum transpose, const float* val){
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniformMatrix3fv(Loc, 1, transpose, val);
		m_uniformLocationDict.insert(std::make_pair(valueName, Loc));
	}
	else {
		glUniformMatrix3fv(iter->second, 1, transpose, val);
	}
}

void ShaderComponent::SetUniformVec4(std::string valueName, const float v1, const float v2, const float v3, const float v4){
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform4f(Loc , v1, v2, v3, v4);
		m_uniformLocationDict.insert(std::make_pair(valueName, Loc));
	}
	else {
		glUniform4f(iter->second, v1, v2, v3, v4);
	}
}

void ShaderComponent::SetUniformVec3(std::string valueName, const float v1, const float v2, const float v3){
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform3f(Loc, v1, v2, v3);
		m_uniformLocationDict.insert(std::make_pair(valueName, Loc));
	}
	else {
		glUniform3f(iter->second, v1, v2, v3);
	}
}

void ShaderComponent::SetUniformVec2(std::string valueName, const float v1, const float v2){
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform2f(Loc, v1, v2);
		m_uniformLocationDict.insert(std::make_pair(valueName, Loc));
	}
	else {
		glUniform2f(iter->second, v1, v2);
	}
}

void ShaderComponent::SetUniformFloat(std::string valueName, const float value){
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform1f(Loc, value);
		m_uniformLocationDict.insert(std::make_pair(valueName, Loc));
	}
	else {
		glUniform1f(iter->second, value);
	}
}

void ShaderComponent::SetUniformInt(std::string valueName, const int value){
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform1i(Loc, value);
		m_uniformLocationDict.insert(std::make_pair(valueName, Loc));
	}
	else {
		glUniform1i(iter->second, value);
	}
}