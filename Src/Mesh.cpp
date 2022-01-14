#include <glad/glad.h>

#include "ClosestPointTest.h"
#include "Utilities.h"

#include "Mesh.h"

namespace PotatoEngine
{
	Mesh::~Mesh()
	{
	}

	void Mesh::Release()
	{
		glDeleteBuffers(1, &m_glVBO);
		glDeleteBuffers(1, &m_glEBO);
		glDeleteVertexArrays(1, &m_glVAO);
	}

	void Mesh::SetupGL()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &m_glVAO);
		glGenBuffers(1, &m_glVBO);
		glGenBuffers(1, &m_glEBO);

		glBindVertexArray(m_glVAO);
		
		// Load data into buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

		// Vertex arribute pointer
		// Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glBindVertexArray(0);
	}

	void Mesh::Draw() const
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(m_glVAO);

		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

		// Set everything back to default
		glBindVertexArray(0);
	}

	void Mesh::DrawVertices() const
	{
		glBindVertexArray(m_glVAO);
		glDrawArrays(GL_POINTS, 0, m_vertices.size());
		glBindVertexArray(0);
	}

	glm::vec3 Mesh::ClosestPointOnTriangle(const glm::vec3& p, std::size_t triangleIndex) const
	{
		const auto& triangle = m_triangles[triangleIndex];
		const auto& v0 = m_vertices[triangle.v[0]].Position;
		const auto& v1 = m_vertices[triangle.v[1]].Position;
		const auto& v2 = m_vertices[triangle.v[2]].Position;

		return PotatoEngine::ClosestPointOnTriangle(p, v0, v1, v2);
	}

} // namespace PotatoEngine
