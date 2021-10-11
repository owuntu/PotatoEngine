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

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
		{
			m_vertices = vertices;
			m_indices = indices;
		}
	};
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MESH_H_
