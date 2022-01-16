// Trianglated mesh
#ifndef POTATO_ENGINE_MESH_H_
#define POTATO_ENGINE_MESH_H_
#include <cstdint>
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
			uint32_t v[3];
		};

		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;

		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices):
			m_glVAO(0),
			m_glVBO(0),
			m_glEBO(0)
		{
			m_vertices = vertices;
			m_indices = indices;

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

		// Assuming all indices is for triangulated mesh
		std::size_t NumTriangles() const { return m_indices.size() / 3; }
		const Triangle GetTriangle(std::size_t i) const
		{
			Triangle t;
			auto ti = i * 3;
			t.v[0] = m_indices[ti + 0];
			t.v[1] = m_indices[ti + 1];
			t.v[2] = m_indices[ti + 2];
			return t;
		}

		std::size_t NumVertices() const { return m_vertices.size(); }
		const Vertex& GetVertex(std::size_t i) const { return m_vertices[i]; }

		std::size_t NumIndices() const { return m_indices.size(); }
		const uint32_t GetVertexIndex(std::size_t i) const { return m_indices[i]; }

	private:
		uint32_t m_glVAO;
		uint32_t m_glVBO;
		uint32_t m_glEBO; // element buffer object

		void SetupGL();
	};
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MESH_H_
