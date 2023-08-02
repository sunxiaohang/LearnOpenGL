#pragma once
#include <unordered_map>
#include "glm/glm.hpp"

enum class ShaderType {
	None = -1,
	Vertex = 0,
	Fragment = 1,
};

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	unsigned int m_RendererID;
	const std::string m_FilePath;
	std::unordered_map<std::string, unsigned int> m_shaderLocationMap;

public:
	Shader(const std::string& filePath);
	~Shader();
	void Bind() const;
	void UnBind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	ShaderProgramSource ParseShader(const std::string filePath);
	unsigned int CreateShader(const std::string& vertexShaderStr, const std::string& fragmentShaderStr);
	unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSource);
private:
	int GetUniformLocation(const std::string& name);
};