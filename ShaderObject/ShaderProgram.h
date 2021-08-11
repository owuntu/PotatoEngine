#pragma once

#include <vector>

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
private:
	bool AttachShader(const ShaderObject& shader);
	bool Link();
	unsigned int m_glPrgmId = 0;
	std::vector<unsigned int> m_shaderIDs;
};

} // namespace PotatoEngine
