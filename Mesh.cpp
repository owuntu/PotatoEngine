#include <glad/glad.h>

#include "Mesh.h"

namespace PotatoEngine
{
	void Mesh::SetupGL()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &m_glVAO);
		glGenBuffers(1, &m_glVBO);

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
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glBindVertexArray(0);
	}

	void Mesh::Draw() const
	{
		// Draw mesh
		glBindVertexArray(m_glVAO);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

		// Set everything back to default
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
} // namespace PotatoEngine
