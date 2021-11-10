#ifndef POTATO_ENGINE_MESH_H_
#define POTATO_ENGINE_MESH_H_

#include <vector>
#include <glm/vec3.hpp>

namespace PotatoEngine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
	};

	class Mesh
	{
	public:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		unsigned int m_glVAO;

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices):
			m_glVAO(0),
			m_glVBO(0),
			m_glEBO(0)
		{
			m_vertices = vertices;
			m_indices = indices;

			SetupGL();
		}

		~Mesh();

		void Draw() const;
		void Release();

	private:
		unsigned int m_glVBO;
		unsigned int m_glEBO; // element buffer object

		void SetupGL();
	};
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MESH_H_
