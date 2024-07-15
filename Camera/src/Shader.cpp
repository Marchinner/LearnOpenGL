#include "../lib/Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		std::cout << "SUCESSO::VERTEX_SHADER::ARQUIVO_CARREGADO" << std::endl;
		fragmentCode = fShaderStream.str();
		std::cout << "SUCESSO::FRAGMENT_SHADER::ARQUIVO_CARREGADO" << std::endl;

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERRO::SHADER::ARQUIVO_NAO_PODE_SER_LIDO" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile Shaders
	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	// Vertex
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERRO::VERTEX_SHADER::COMPILACAO_FALHOU\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCESSO::VERTEX_SHADER::COMPILADO" << std::endl;
	}

	// Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERRO::FRAGMENT_SHADER::COMPILACAO_FALHOU\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCESSO::FRAGMENT_SHADER::COMPILADO" << std::endl;
	}

	// Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERRO::SHADER_PROGRAM::FALHA_AO_LINKAR\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCESSO::SHADER_PROGRAM::LINKADO" << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setFloat4(const std::string& name, float value[4]) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value[0], value[1] ,value[2], value[3]);
}