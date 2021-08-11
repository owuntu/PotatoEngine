#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include "ShaderObject.h"

namespace PotatoEngine
{

ShaderObject::ShaderObject() :
	m_glid(0)
{
}

ShaderObject::~ShaderObject()
{
}

bool ShaderObject::CreateShader(const char* src, ShaderType sType)
{
	GLuint shaderType = 0;
	switch (sType)
	{
	case ShaderType::VertexShader:
		shaderType = GL_VERTEX_SHADER;
		break;
	case ShaderType::FragmentShader:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	default:
		break;
	}

	if (shaderType == 0)
	{
		std::cerr << "Unsupported shader type.\n";
		return false;
	}

	m_shaderSource = src;
	m_glid = glCreateShader(shaderType);
	glShaderSource(m_glid, 1, &src, NULL);
	glCompileShader(m_glid);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(m_glid, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_glid, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(m_glid);
		m_glid = 0;
		m_shaderSource.clear();
		return false;
	}

	return true;
}

bool ShaderObject::CreateShaderFromFile(const std::string filePath, ShaderType sType)
{
	using namespace std;
	fstream file;
	file.open(filePath, ios::in);
	if (file.is_open() == false)
	{
		cerr << "Cannot open file: " << filePath << std::endl;
		return false;
	}

	std::size_t fileSize = 0;
	file.seekg(0, ios::end);
	fileSize = file.tellg();
	m_shaderSource.resize(fileSize);
	file.seekg(0);
	file.read(&m_shaderSource[0], fileSize);
	file.close();

	std::cout << "Input shader source:\n" << m_shaderSource << std::endl;

	return this->CreateShader(m_shaderSource.c_str(), sType);
}

void ShaderObject::Release()
{
	if (m_glid != 0)
	{
		glDeleteShader(m_glid);
		m_glid = 0;
		m_shaderSource.clear();
	}
}

} // namespace PotatoEngine
