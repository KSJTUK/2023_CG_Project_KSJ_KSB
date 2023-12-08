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

	m_shaderList[static_cast<int>(ShaderType::StaticShader)] = std::make_shared<ShaderComponent>(static_shader);
	m_shaderList[static_cast<int>(ShaderType::TerrainShader)] = std::make_shared<ShaderComponent>(terrain_shader);
	m_shaderList[static_cast<int>(ShaderType::BackgroundShader)] = std::make_shared<ShaderComponent>(background_shader);
	m_shaderList[static_cast<int>(ShaderType::AnimatedShader)] = std::make_shared<ShaderComponent>(animated_shader);
}




Shader* Shader::GetInstance(){
	if (m_instance == nullptr) {
		m_instance = new Shader{};
	}

	return m_instance;
}


std::shared_ptr<ShaderComponent> Shader::GetActivatedShader() const{

	if (m_activatedShader == nullptr) {
		std::cerr << "Error to get NULL Shaer" << std::endl;
		exit(EXIT_FAILURE);
	}


	return m_activatedShader;
}


void Shader::UseProgram(ShaderType type){
	m_shaderList[static_cast<int>(type)]->UseProgram();
	m_activatedShader = m_shaderList[static_cast<int>(type)];
}

void Shader::UnuseProgram() {
	glUseProgram(INVALID);
}



int ShaderComponent::GetUniformLocation(const std::string& valueName) {
	 
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

UINT ShaderComponent::CreateVertexShader(const std::string& path){

	UINT id{};
	
	GLchar errorlog[512]{};


	std::ifstream file = std::ifstream((shaderDir +  path).c_str(), std::ios::in);
	std::stringstream ss = std::stringstream{};
	std::string source{};

	if (file.is_open()) {
		ss << file.rdbuf();
		source = ss.str();
	}
	else {
		std::cerr << "ERROR : FAILED TO OPEN FILE " << std::endl;
		exit(EXIT_FAILURE);
	}


	const char* source_cstr = source.c_str();

	id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(id, 1, &source_cstr, NULL);
	glCompileShader(id);


	GLint result{};
	ZeroMemory(errorlog, 512);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 512, NULL, errorlog);
		std::cerr << "ERROR : VERTEX SHADER COMPILE ERROR" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "VERTEX SHADER SUCESSFULLY COMPILED!" << std::endl;
	}




	return id;
}

UINT ShaderComponent::CreateGeometryShader(const std::string& path){

	UINT id{};

	GLchar errorlog[512]{};


	std::ifstream file = std::ifstream((shaderDir + path).c_str(), std::ios::in);
	std::stringstream ss = std::stringstream{};
	std::string source{};

	if (file.is_open()) {
		ss << file.rdbuf();
		source = ss.str();
	}
	else {
		std::cerr << "ERROR : FAILED TO OPEN FILE " << std::endl;
		exit(EXIT_FAILURE);
	}


	const char* source_cstr = source.c_str();

	id = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(id, 1, &source_cstr, NULL);
	glCompileShader(id);


	GLint result{};
	ZeroMemory(errorlog, 512);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 512, NULL, errorlog);
		std::cerr << "ERROR : GEOMETRY SHADER COMPILE ERROR" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "GEOMETRY SHADER SUCESSFULLY COMPILED!" << std::endl;
	}




	return id;
}

UINT ShaderComponent::CreateTesselationControlShader(const std::string& path){
	UINT id{};

	GLchar errorlog[512]{};


	std::ifstream file = std::ifstream((shaderDir + path).c_str(), std::ios::in);
	std::stringstream ss = std::stringstream{};
	std::string source{};

	if (file.is_open()) {
		ss << file.rdbuf();
		source = ss.str();
	}
	else {
		std::cerr << "ERROR : FAILED TO OPEN FILE " << std::endl;
		exit(EXIT_FAILURE);
	}


	const char* source_cstr = source.c_str();

	id = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(id, 1, &source_cstr, NULL);
	glCompileShader(id);


	GLint result{};
	ZeroMemory(errorlog, 512);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 512, NULL, errorlog);
		std::cerr << "ERROR : TESSELATION CONTROL SHADER COMPILE ERROR" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "TESSELATION CONTROL SHADER SUCESSFULLY COMPILED!" << std::endl;
	}




	return id;


}

UINT ShaderComponent::CreateTesselationEvaluationShader(const std::string& path){
	UINT id{};

	GLchar errorlog[512]{};


	std::ifstream file = std::ifstream((shaderDir + path).c_str(), std::ios::in);
	std::stringstream ss = std::stringstream{};
	std::string source{};

	if (file.is_open()) {
		ss << file.rdbuf();
		source = ss.str();
	}
	else {
		std::cerr << "ERROR : FAILED TO OPEN FILE " << std::endl;
		exit(EXIT_FAILURE);
	}


	const char* source_cstr = source.c_str();

	id = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(id, 1, &source_cstr, NULL);
	glCompileShader(id);


	GLint result{};
	ZeroMemory(errorlog, 512);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 512, NULL, errorlog);
		std::cerr << "ERROR : TESSELATION EVALUATION SHADER COMPILE ERROR" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "TESSELATION EVALUATION SHADER SUCESSFULLY COMPILED!" << std::endl;
	}




	return id;


}

UINT ShaderComponent::CreateFragmentShader(const std::string& path){

	UINT id{};

	GLchar errorlog[512]{};


	std::ifstream file = std::ifstream((shaderDir + path).c_str(), std::ios::in);
	std::stringstream ss = std::stringstream{};
	std::string source{};

	if (file.is_open()) {
		ss << file.rdbuf();
		source = ss.str();
	}
	else {
		std::cerr << "ERROR : FAILED TO OPEN FILE " << std::endl;
		exit(EXIT_FAILURE);
	}


	const char* source_cstr = source.c_str();

	id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(id, 1, &source_cstr, NULL);
	glCompileShader(id);


	GLint result{};
	ZeroMemory(errorlog, 512);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 512, NULL, errorlog);
		std::cerr << "ERROR : FRAGMENT SHADER COMPILE ERROR" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "FRAGMENT SHADER SUCESSFULLY COMPILED!" << std::endl;
	}




	return id;
}


ShaderComponent::ShaderComponent(const std::vector<std::string>& files) {

	UINT VertexProgramID{INVALID};
	UINT GeometrtyProgramID{ INVALID };
	UINT TesselationControlProgramID{ INVALID };
	UINT TesselationEvaluationProgramID{ INVALID };
	UINT FragmentProgramID{ INVALID };



	m_id = glCreateProgram();
	
	std::cout << "=======================================================================================" << std::endl;

	for (auto i = 0; i < files.size(); ++i) {
		if (files[i] == EMPTYSTRING) continue;
		switch (i){
		case 0:
			VertexProgramID = CreateVertexShader(files[i]);
			glAttachShader(m_id, VertexProgramID);
			break;
		case 1:
			GeometrtyProgramID = CreateGeometryShader(files[i]);
			glAttachShader(m_id, GeometrtyProgramID);
			break;
		case 2:
			TesselationControlProgramID = CreateTesselationControlShader(files[i]);
			glAttachShader(m_id, TesselationControlProgramID);
			break;
		case 3:
			TesselationEvaluationProgramID = CreateTesselationEvaluationShader(files[i]);
			glAttachShader(m_id, TesselationEvaluationProgramID);
			break;
		case 4:
			FragmentProgramID = CreateFragmentShader(files[i]);
			glAttachShader(m_id, FragmentProgramID);
			break;
		default:
			break;
		}
	}

	std::cout << "=======================================================================================" << std::endl;

	glLinkProgram(m_id);



	if (VertexProgramID != INVALID) glDeleteProgram(VertexProgramID);
	if (GeometrtyProgramID != INVALID) glDeleteProgram(GeometrtyProgramID);
	if (TesselationControlProgramID != INVALID) glDeleteProgram(TesselationControlProgramID);
	if (TesselationEvaluationProgramID != INVALID) glDeleteProgram(TesselationEvaluationProgramID);
	if (FragmentProgramID != INVALID) glDeleteProgram(FragmentProgramID);



	GLchar errorlog[512]{};
	
	ZeroMemory(errorlog, 512);
	GLint result{};
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);

	if (!result) {
		ZeroMemory(errorlog, 512);
		glGetProgramInfoLog(result, 512, NULL, errorlog);
		std::cerr << "ERROR : SHADER LINK FAILED" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "SHADERS SUCESSFULLY CREATED" << std::endl;
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