#include <iostream>
#include <glad/glad.h>

#include "GLObject.h"

namespace PotatoEngine
{
	GLObject::GLObject() :
		m_glid(0),
		m_pCounter(nullptr),
		m_num(1),
		m_type(ObjectType::Count)
	{}

	GLObject::GLObject(GLObject& other) :
		m_glid(other.m_glid),
		m_pCounter(other.m_pCounter),
		m_num(other.m_num),
		m_type(other.m_type)
	{
		if (m_pCounter != nullptr)
		{
			*m_pCounter++;
		}
	}

	GLObject::~GLObject()
	{
		Release();
		if (m_pCounter != nullptr && *m_pCounter <= 0)
		{
			delete m_pCounter;
			m_pCounter = nullptr;
		}
	}

	unsigned int GLObject::InitGLObject(ObjectType type, unsigned int num)
	{
		if (m_pCounter != nullptr)
		{
			Release();
		}

		m_num = num;
		switch (type)
		{
		case ObjectType::Buffer:
			glGenBuffers(m_num, &m_glid);
			break;
		default:
			std::cout << "Unspported GLObject type: " << (unsigned int)type << std::endl;
			break;
		}

		m_pCounter = new int(1);

		return m_glid;
	}

	void GLObject::Release()
	{
		if (m_pCounter != nullptr)
		{
			m_pCounter--;
			if (*m_pCounter <= 0 && m_glid != 0)
			{
				switch (m_type)
				{
				case ObjectType::Buffer:
					glDeleteBuffers(m_num, &m_glid);
					break;
				default:
					break;
				}
				delete m_pCounter;
				m_pCounter = nullptr;
				m_glid = 0;
				m_num = 1;
			}
		}
	}

	unsigned int GLObject::GetGLID() const
	{
		return m_glid;
	}

} // namespace PotatoEngine
