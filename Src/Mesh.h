// Trianglated mesh
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
		struct Triangle
		{
			// vertex indices
			unsigned int v[3];
		};

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<Triangle> m_triangles;

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices):
			m_glVAO(0),
			m_glVBO(0),
			m_glEBO(0)
		{
			m_vertices = vertices;
			m_indices = indices;

			for (std::size_t i = 0; i < m_indices.size(); i += 3)
			{
				Triangle t;
				t.v[0] = m_indices[i];
				t.v[1] = m_indices[i + 1];
				t.v[2] = m_indices[i + 2];

				m_triangles.push_back(t);
			}

			SetupGL();
		}

		Mesh() :
			m_glVAO(0),
			m_glVBO(0),
			m_glEBO(0)
		{}

		~Mesh();

		void Draw() const;
		void DrawVertices() const;
		void Release();

		glm::vec3 ClosestPointOnTriangle(const glm::vec3& p, std::size_t triangleIndex) const;

		std::size_t NumTriangles() const { return m_triangles.size(); }
		const Triangle& GetTriangle(std::size_t i) const { return m_triangles[i]; }

		std::size_t NumVertices() const { return m_vertices.size(); }
		const Vertex& GetVertex(std::size_t i) const { return m_vertices[i]; }

		std::size_t NumIndices() const { return m_indices.size(); }
		const unsigned int GetIndex(std::size_t i) const { return m_indices[i]; }

	private:
		unsigned int m_glVAO;
		unsigned int m_glVBO;
		unsigned int m_glEBO; // element buffer object

		void SetupGL();
	};
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MESH_H_
