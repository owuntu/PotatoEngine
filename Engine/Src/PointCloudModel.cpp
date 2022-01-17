

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>


#include "PointCloudModel.h"

namespace PotatoEngine
{
	void PointCloudModel::ProcessMesh(aiMesh* pMesh)
	{
		for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
		{
			glm::vec3 v;
			v.x = pMesh->mVertices[i].x;
			v.y = pMesh->mVertices[i].y;
			v.z = pMesh->mVertices[i].z;
			
			m_points.push_back(v);
		}
	}

	// Setup necessary OpenGL buffer in PostSetup step
	void PointCloudModel::PostSetup()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &m_glVAO);
		glGenBuffers(1, &m_glVBO);

		glBindVertexArray(m_glVAO);

		// Load data into buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
		glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(glm::vec3), &m_points[0], GL_STATIC_DRAW);

		// Vertex arribute pointer
		// Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindVertexArray(0);
	}

	void PointCloudModel::DoDraw() const
	{
		glPointSize(1.5f);
		glBindVertexArray(m_glVAO);
		glDrawArrays(GL_POINTS, 0, m_points.size());
		glBindVertexArray(0);
		glPointSize(1.0f);
	}


} // namespace PotatoEngine
