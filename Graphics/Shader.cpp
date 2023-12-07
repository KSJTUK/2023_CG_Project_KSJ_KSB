#include "pch.h"
#include "Shader.h"


const std::string shaderDir{ "./Shader/" };
const std::string EMPTYSTRING{ };


const std::vector<std::string> static_shader{
	"static_vertex_shader.glsl",
	EMPTYSTRING,
	EMPTYSTRING,
	EMPTYSTRING,
	"static_fragment_shader.glsl"
};

const std::vector<std::string> animated_shader{
	"animated_vertex_shader.glsl",
	EMPTYSTRING,
	EMPTYSTRING,
	EMPTYSTRING,
	"animated_fragment_shader.glsl"
};

const std::vector<std::string> background_shader{
	"background_vertex_shader.glsl",
	EMPTYSTRING,
	EMPTYSTRING,
	EMPTYSTRING,
	"background_fragment_shader.glsl"
};

const std::vector<std::string> terrain_shader{
	"terrain_vertex_shader.glsl",
	EMPTYSTRING,
	"terrain_tessel_control_shader.glsl",
	"terrain_tessel_evaluation_shader.glsl",
	"terrain_fragment_shader.glsl"
};

const std::vector<std::string> light_shader{
	"light_object_vertex_shader.glsl",
	EMPTYSTRING,
	EMPTYSTRING,
	EMPTYSTRING,
	"light_object_fragment_shader.glsl"
};

const std::vector<std::string> particle_shader{
	"particle_vertex_shader.glsl",
	"particle_geometry_shader.glsl",
	EMPTYSTRING,
	EMPTYSTRING,
	"particle_fragment_shader.glsl"
};

const std::vector<std::string> ui_shader{
	"ui_vertex_shader.glsl",
	EMPTYSTRING,
	EMPTYSTRING,
	EMPTYSTRING,
	"ui_fragment_shader.glsl"
};

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



int ShaderComponent::GetUniformLocation(const std::string& valueName){
	 
	auto result = m_uniformLocationDict.find(valueName);

	int location{};

	if (result == m_uniformLocationDict.end()) {
		location = glGetUniformLocation(m_id, valueName.c_str());
		m_uniformLocationDict.insert(std::make_pair(valueName, location));
#ifdef _DEBUG
		assert(location != -1);
#else // Release
	if (location == -1) {
		std::cerr << "Error in\nFile : " << __FILE__ << "\n" << "Line : " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
#endif
		return location;
	}
	else {
		return result->second;
	}
}





void ShaderComponent::SetUniformMat4(const std::string& valueName, GLenum transpose, const float* val){
	glUniformMatrix4fv(GetUniformLocation(valueName), 1, transpose, val);
}

void ShaderComponent::SetUniformMat3(const std::string& valueName, GLenum transpose, const float* val){
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

void ShaderComponent::SetUniformVec4(const std::string& valueName, const float* val){
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform4fv(Loc, 1, val);
	}
}


void ShaderComponent::SetUniformVec3(const std::string& valueName, const float* val) {
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform3fv(Loc, 1, val);
	}
}


void ShaderComponent::SetUniformVec2(const std::string& valueName, const float* val) {
	auto iter = m_uniformLocationDict.find(valueName);

	if (iter == m_uniformLocationDict.end()) {
		int Loc = GetUniformLocation(valueName);
		glUniform2fv(Loc, 1, val);
	}
}



void ShaderComponent::SetUniformFloat(const std::string& valueName, const float value){
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
	
void ShaderComponent::SetUniformInt(const std::string& valueName, const int value){
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