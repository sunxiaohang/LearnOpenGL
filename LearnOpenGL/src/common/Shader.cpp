#include "Utils.h"
#include "Shader.h"

Shader::Shader(const std::string& filePath):m_FilePath(filePath),m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLTryCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLTryCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLTryCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLTryCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLTryCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	GLTryCall(glUniform4f(location, f0, f1, f2, f3));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLTryCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLTryCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if(m_shaderLocationMap.find(name) != m_shaderLocationMap.end()) return m_shaderLocationMap[name];
	GLTryCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout<<"warning: uniform [" << name <<"] does not exist" <<std::endl;
	}
	else
	{
		m_shaderLocationMap[name] = location;
	}
	return location;
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& shaderSource)
{
	unsigned int id = glCreateShader(shaderType);
	const char* src = shaderSource.c_str();
	GLTryCall(glShaderSource(id, 1, &src, nullptr));
	GLTryCall(glCompileShader(id));
	int result;
	GLTryCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLTryCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(sizeof(char) * length);
		GLTryCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile" << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << message << std::endl;
		GLTryCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShaderStr, const std::string& fragmentShaderStr)
{
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderStr);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderStr);

	unsigned int shaderProgram = glCreateProgram();
	GLTryCall(glAttachShader(shaderProgram, vertexShader));
	GLTryCall(glAttachShader(shaderProgram, fragmentShader));
	GLTryCall(glLinkProgram(shaderProgram));
	GLTryCall(glValidateProgram(shaderProgram));

	GLTryCall(glDeleteShader(vertexShader));
	GLTryCall(glDeleteShader(fragmentShader));
	return shaderProgram;
}

ShaderProgramSource Shader::ParseShader(const std::string filePath)
{
	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::None;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::Vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::Fragment;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}
