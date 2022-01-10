#include <algorithm>
#include <stack>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>

#include <glm/gtx/norm.hpp>

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

		Build();
	}

	void PointCloudModel::DoDraw() const
	{
		this->DoDrawVertices();
	}

	void PointCloudModel::DoDrawVertices() const
	{
		glPointSize(1.5f);
		glBindVertexArray(m_glVAO);
		glDrawArrays(GL_POINTS, 0, m_points.size());
		glBindVertexArray(0);
		glPointSize(1.0f);
	}

	// KD tree methods
	void PointCloudModel::Sort(int start, int end, int axis, Node* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}

		std::sort(m_tmpElements.begin() + start, m_tmpElements.begin() + end,
			[&](int a, int b)
			{
				return m_points[a][axis] < m_points[b][axis];
			}
		);

		pNode->splitAxis = axis;
		glm::vec3 splitPoint = m_points[m_tmpElements[(end + start) / 2]];
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

	int PointCloudModel::GetSplitAxis(int start, int end, int depth)
	{
		int axis = depth % 3;
		return axis;

		// Doesn't see improvement using below method to decide split axis
		//return GetSplitAxisRange(start, end);
		//return GetSplitAxisVariance(start, end);
	}

	int PointCloudModel::GetSplitAxisRange(int start, int end)
	{
		glm::vec3 vmax(-FLT_MAX);
		glm::vec3 vmin(FLT_MAX);

		for (int i = start; i < end; ++i)
		{
			auto point = m_points[m_tmpElements[i]];
			for (int x = 0; x < 3; ++x)
			{
				vmax[x] = fmaxf(vmax[x], point[x]);
				vmin[x] = fminf(vmin[x], point[x]);
			}
		}

		glm::vec3 diff = vmax - vmin;
		return (diff[0] > diff[1] && diff[0] > diff[2]) ? 
				0 : (diff[1] > diff[2] ?
						1 : 2);
	}

	int PointCloudModel::GetSplitAxisVariance(int start, int end)
	{
		glm::vec3 mean(0.f);
		glm::vec3 variance(0.f);
		int n = end - start;
		for (int i = start; i < end; ++i)
		{
			auto point = m_points[m_tmpElements[i]];
			mean += point;
		}
		mean /= n;

		for (int i = start; i < end; ++i)
		{
			auto point = m_points[m_tmpElements[i]];
			auto diff = point - mean;
			variance += (diff * diff);
		}
		variance /= n;
		return (variance[0] > variance[1] && variance[0] > variance[2]) ?
				0 : (variance[1] > variance[2] ?
					1 : 2);
	}

	glm::vec3 PointCloudModel::SearchNearest(const glm::vec3& queryPoint)
	{
		float currentMin2 = FLT_MAX;
		return SearchNearest(queryPoint, 0, currentMin2);
		//return SearchNearest(queryPoint, m_root, currentMin2);

		// todo: Incorrect search result
		//return SearchNearestIterate(queryPoint, m_root, currentMin2);
	}

	glm::vec3 PointCloudModel::SearchNearest(const glm::vec3& queryPoint, int offset, float& currentMin2) const
	{
		auto& node = m_nodeArry[offset];
		if (node.bIsLeaf)
		{
			glm::vec3 res(NAN);
			for (auto i : node.elements)
			{
				const glm::vec3& testPoint = m_points[i];
				float d2 = glm::distance2(queryPoint, testPoint);
				if (d2 < currentMin2)
				{
					currentMin2 = d2;
					res = testPoint;
				}
			}

			return res;
		}

		float d = queryPoint[node.nodeData.splitAxis] - node.nodeData.splitPos;
		float d2 = d * d;
		int off2 = offset << 1;
		int left = off2 + 1;
		int right = off2 + 2;
		if (d >= 0.f)
		{
			int t = left;
			left = right;
			right = t;
		}

		auto res = SearchNearest(queryPoint, left, currentMin2);
		if (d2 < currentMin2)
		{
			auto tmp = SearchNearest(queryPoint, right, currentMin2);
			if (!isnan(tmp.x))
			{
				res = tmp;
			}
		}
		return res;
	}


#if 1

	glm::vec3 PointCloudModel::SearchNearest(const glm::vec3& queryPoint, const Node* pNode, float& currentMin2) const
	{
		if (pNode->splitAxis == -1)
		{
			glm::vec3 res(NAN);
			for (auto i : pNode->elements)
			{
				const glm::vec3& testPoint = m_points[i];
				float d2 = glm::distance2(queryPoint, testPoint);
				if (d2 < currentMin2)
				{
					currentMin2 = d2;
					res = testPoint;
				}
			}

			return res;
		}

		float d = queryPoint[pNode->splitAxis] - pNode->splitPos;
		float d2 = d * d;
		Node* left = pNode->left;
		Node* right = pNode->right;
		if (d >= 0.f)
		{
			left = right;
			right = pNode->left;
		}

		auto res = SearchNearest(queryPoint, left, currentMin2);
		if (d2 < currentMin2)
		{
			auto tmp = SearchNearest(queryPoint, right, currentMin2);
			if (!isnan(tmp.x))
			{
				res = tmp;
			}
		}
		return res;
	}
#endif
#if 0
	// todo: Incorrect search result
	glm::vec3 PointCloudModel::SearchNearestIterate(const glm::vec3& queryPoint, const Node* root, float& currentMin2)
	{
		using namespace std;
		
		glm::vec3 res(NAN);

		struct TmpNode
		{
			const Node* treeNode;

			// Distance square between current closest point and the split plane of parent node.
			float d2Parent;

			TmpNode(const Node* node, float d2 = FLT_MAX):
				treeNode(node), d2Parent(d2)
			{}
		};

		stack<TmpNode> nodeStack;
		nodeStack.push(TmpNode(root));

		while (!nodeStack.empty())
		{
			auto node = nodeStack.top();
			const Node* pNode = node.treeNode;
			nodeStack.pop();

			if (pNode->splitAxis == -1)
			{
				for (auto i : pNode->elements)
				{
					const glm::vec3& testPoint = m_points[i];
					float d2 = glm::distance2(queryPoint, testPoint);
					if (d2 < currentMin2)
					{
						currentMin2 = d2;
						res = testPoint;
					}
				}

				// If the distance to the parent split plane is smaller than to the current closest point,
				// we need to continue to traverse the sibiling node
				if (node.d2Parent < currentMin2 || isnan(res.x))
				{
					continue;
				}

				break;
			}

			// distance to split plane
			float d = queryPoint[pNode->splitAxis] - pNode->splitPos;
			float d2 = d * d;

			Node* left = pNode->left;
			Node* right = pNode->right;

			// Traverse right node first if queryPoint is in the greater side of the split plane
			if (d >= 0.f)
			{
				left = right;
				right = pNode->left;
			}

			nodeStack.push(TmpNode(right));
			nodeStack.push(TmpNode(left, d2));
		}

		return res;
	}
#endif

} // namespace PotatoEngine
