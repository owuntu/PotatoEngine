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

	void SetMat4(const std::string& name, const glm::mat4& mat);

	void SetVec3(const std::string& name, const glm::vec3& vec);
	void SetVec4(const std::string& name, const glm::vec4& vec);

	void SetInt(const std::string& name, const int v);

private:
	bool AttachShader(const ShaderObject& shader);
	bool Link();
	unsigned int m_glPrgmId = 0;
	std::vector<unsigned int> m_shaderIDs;
};

} // namespace PotatoEngine
