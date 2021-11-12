#pragma once

namespace PotatoEngine
{
	class GLObject
	{
	public:
		enum class ObjectType : unsigned int
		{
			Buffer = 0,
			Array,
			Texture,
			Count
		};

		GLObject();
		GLObject(GLObject&);
		~GLObject();

		unsigned int InitGLObject(ObjectType, unsigned int num = 1);
		void Release();

		unsigned int GetGLID() const;

	private:
		unsigned int m_glid;
		ObjectType m_type;
		unsigned int m_num; // number of gl objects that gl id holding.
		int* m_pCounter; // reference counter
	};
} // namespace PotatoEngine

