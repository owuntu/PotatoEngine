#pragma once

#include <vector>

namespace PotatoEngine
{

class ShaderObject;

class ShaderProgram
{
public:
	bool Create();
	bool AttachShader(const ShaderObject& shader);
	bool Link();
	void Release();

	unsigned int ProgramID() const { return m_glPrgmId; }
private:
	unsigned int m_glPrgmId = 0;
	std::vector<unsigned int> m_shaderIDs;
};

} // namespace PotatoEngine
