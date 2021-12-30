#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>

#include <algorithm>
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

		KdTree::Build();
	}

	void PointCloudModel::DoDraw() const
	{
		this->DoDrawVertices();
	}

	void PointCloudModel::DoDrawVertices() const
	{
		glPointSize(1.0f);
		glBindVertexArray(m_glVAO);
		glDrawArrays(GL_POINTS, 0, m_points.size());
		glBindVertexArray(0);
	}

	void PointCloudModel::Sort(std::vector<int>& elements, int axis, Node* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}

		std::sort(elements.begin(), elements.end(),
			[&](int a, int b)
			{
				return m_points[a][axis] < m_points[b][axis];
			}
		);

		pNode->splitAxis = axis;
		glm::vec3 splitPoint = m_points[elements[elements.size() / 2]];
		pNode->splitPos = splitPoint[axis];
	}

	int PointCloudModel::GetTotalNumOfElements() const
	{
		return m_points.size();
	}

	BBox PointCloudModel::GetBoundingBox(const std::vector<int>& elements) const
	{
		BBox box;
		box.Init();
		for (auto i : elements)
		{
			box.Add(m_points[i]);
		}
		return box;
	}

} // namespace PotatoEngine
