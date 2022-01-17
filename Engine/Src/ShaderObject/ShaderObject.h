#pragma once

#include <string>

namespace PotatoEngine
{

class ShaderObject
{
public:
	ShaderObject();
	~ShaderObject();

	enum class ShaderType : unsigned int
	{
		Undefine = 0,
		VertexShader,
		FragmentShader,
		Count
	};

	bool CreateShader(const char* src, ShaderType sType);
	bool CreateShaderFromFile(const std::string filePath, ShaderType sType);
	void Release();

	unsigned int ShaderID() const { return m_glid; }
private:
	std::string m_shaderSource;
	unsigned int m_glid;

	friend class ShaderProgram;
};

} // namespace PotatoEngine
