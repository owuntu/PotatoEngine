#include <glad/glad.h>

#include "SinglePointModel.h"

namespace PotatoEngine
{
	void SinglePointModel::PostSetup()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &m_glVAO);
		glGenBuffers(1, &m_glVBO);

		glBindVertexArray(m_glVAO);

		// Load data into buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &m_point, GL_STATIC_DRAW);

		// Vertex arribute pointer
		// Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindVertexArray(0);
	}

	void SinglePointModel::Draw() const
	{
		this->DrawVertices();
	}

	void SinglePointModel::DrawVertices() const
	{
		glPointSize(10.0f);
		glBindVertexArray(m_glVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

} // namespace PotatoEngine
