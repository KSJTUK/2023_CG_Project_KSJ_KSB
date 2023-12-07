#pragma once
	
constexpr unsigned int INVALID = 0xFFFFFFFF;


class ShaderComponent {
public:
	ShaderComponent() = default;
	ShaderComponent(const std::vector<std::string>& files);

	const unsigned int Get() const { return m_id; };

	void UseProgram() { glUseProgram(m_id); };
	
	// Uniform
	// 모든 변수는 표준 자료형을 통해 받아올 것. 
	
	void SetUniformMat4(const std::string& valueName, GLenum transpose, const float* val);
	void SetUniformMat3(const std::string& valueName, GLenum transpose, const float* val);
	// MEMO:
	// 아직은 Mat2 가 필요하다고 판단되지 않아서 추가 안함. 필요한경우 같은 포맷으로 추가하길 바람 

	void SetUniformVec4(const std::string& valueName, const float* val);
	void SetUniformVec3(const std::string& valueName, const float* val);
	void SetUniformVec2(const std::string& valueName, const float* val);


	void SetUniformFloat(const std::string& valueName, const float value);
	void SetUniformInt(const std::string& valueName, const int value);


private:
	
	int GetUniformLocation(const std::string& valName);



private:


	std::unordered_map<std::string, UINT> m_uniformLocationDict{};
	UINT m_id{};

};

enum class ShaderType {
	StaticShader,
	AnimateShader,
	SkyboxShader
};

class Shader {
private:
	Shader() = default;
	~Shader() = default;
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

public:

	static Shader* GetInstance();

private:

	static Shader* m_instance;


private:

	std::vector<std::shared_ptr<ShaderComponent>> m_shaderList{};
	std::shared_ptr<ShaderComponent> m_activatedShader{ nullptr };


public:


	void Initialize();

	void UseProgram(ShaderType type);

	void UnuseProgram();

	
	std::shared_ptr<ShaderComponent> GetActivatedShader() const { return m_activatedShader;  }
};

#define SHADER Shader::GetInstance()