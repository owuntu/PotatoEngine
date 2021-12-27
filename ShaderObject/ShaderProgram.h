#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

namespace PotatoEngine
{

class ShaderObject;

class ShaderProgram
{
public:
	bool Create(
		const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath);
	void Use();
	void Release();

	unsigned int ProgramID() const { return m_glPrgmId; }

	void SetMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_glPrgmId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetVec3(const std::string& name, const glm::vec3& vec)
	{
		glUniform3fv(glGetUniformLocation(m_glPrgmId, name.c_str()), 1, &vec[0]);
	}

private:
	bool AttachShader(const ShaderObject& shader);
	bool Link();
	unsigned int m_glPrgmId = 0;
	std::vector<unsigned int> m_shaderIDs;
};

} // namespace PotatoEngine
