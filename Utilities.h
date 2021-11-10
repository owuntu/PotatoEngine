#pragma once

#include <glad/glad.h>
#include <iostream>

namespace PotatoEngine
{
	inline void CheckGLError()
	{
		GLenum errCode = glGetError();
		if (errCode != GL_NO_ERROR)
		{
			std::cout << "GL Error: " << std::hex << errCode << std::endl;
		}
	}
} // namespace PotatoEngine
