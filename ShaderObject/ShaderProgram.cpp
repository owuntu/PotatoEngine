#include <iostream>
#include <glad/glad.h>
#include "ShaderObject.h"

#include "ShaderProgram.h"

namespace PotatoEngine
{

bool ShaderProgram::Create(
	const std::string& vertexShaderPath,
	const std::string& fragmentShaderPath)
{
	if (m_glPrgmId != 0)
	{
		glDeleteProgram(m_glPrgmId);
		m_glPrgmId = 0;
	}
	m_glPrgmId = glCreateProgram();

	bool bSuccess;
	ShaderObject vertexShader;
	ShaderObject fragmentShader;
	bSuccess = vertexShader.CreateShaderFromFile(vertexShaderPath, ShaderObject::ShaderType::VertexShader);
	if (!bSuccess)
	{
		glDeleteProgram(m_glPrgmId);
		m_glPrgmId = 0;
		return false;
	}
	
	bSuccess = fragmentShader.CreateShaderFromFile(fragmentShaderPath, ShaderObject::ShaderType::FragmentShader);
	if (!bSuccess)
	{
		glDeleteProgram(m_glPrgmId);
		m_glPrgmId = 0;
		return false;
	}

	bSuccess &= this->AttachShader(vertexShader);
	bSuccess &= this->AttachShader(fragmentShader);

	bSuccess &= this->Link();

	if (!bSuccess)
	{
		return false;
	}

	vertexShader.Release();
	fragmentShader.Release();

	return true;
}

bool ShaderProgram::AttachShader(const ShaderObject& shader)
{
	if (m_glPrgmId == 0)
	{
		return false;
	}

	for (const auto& iShader : m_shaderIDs)
	{
		if (iShader == shader.ShaderID())
		{
			return true; // Already exist
		}
	}

	glAttachShader(m_glPrgmId, shader.ShaderID());

	m_shaderIDs.push_back(shader.ShaderID());
	return true;
}

bool ShaderProgram::Link()
{
	if (m_glPrgmId == 0 || m_shaderIDs.size() == 0)
	{
		return false;
	}

	glLinkProgram(m_glPrgmId);

	int success;
	char infoLog[512];
	// check for linking errors
	glGetProgramiv(m_glPrgmId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_glPrgmId, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}
	return true;
}

void ShaderProgram::Use()
{
	glUseProgram(m_glPrgmId);
}

void ShaderProgram::Release()
{
	if (m_glPrgmId != 0)
	{
		for (auto& shader : m_shaderIDs)
		{
			glDetachShader(m_glPrgmId, shader);
		}
		glDeleteProgram(m_glPrgmId);
		m_glPrgmId = 0;
		m_shaderIDs.clear();
	}
}

}
