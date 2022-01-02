#ifndef POTATO_ENGINE_POINT_CLOUD_MODEL_H_
#define POTATO_ENGINE_POINT_CLOUD_MODEL_H_

#include <vector>

#include <glm/glm.hpp>

#include "Model.h"
#include "KdTree.h"

namespace PotatoEngine
{
	class PointCloudModel : public Model, public KdTree
	{
	public:
		PointCloudModel() :
			m_glVAO(0),
			m_glVBO(0)
		{}

		~PointCloudModel() {}

		const std::vector<glm::vec3>& GetPoints() const { return m_points; }
		std::vector<glm::vec3>& GetPoints() { return m_points; }

		glm::vec3 SearchNearest(const glm::vec3& queryPoint);

	protected:
		// All mesh vertex point are just put together
		std::vector<glm::vec3> m_points;

	private:
		void ProcessMesh(aiMesh* pMesh);

		void PostSetup();

		void DoDraw() const;
		void DoDrawVertices() const;


		// KdTree method
		void Sort(int start, int end, int axis, Node* node);
		int GetTotalNumOfElements() const;
		BBox GetBoundingBox(const std::vector<int>& elements) const;
		int GetSplitAxis(int start, int end, int depth);
		
		int GetSplitAxisRange(int start, int end);
		int GetSplitAxisVariance(int start, int end);
		
		// Recursive version
		glm::vec3 SearchNearest(const glm::vec3& queryPoint, const Node* pNode, float& currentMin2);
		
		// Iteration version
		//glm::vec3 SearchNearestIterate(const glm::vec3& queryPoint, const Node* pNode, float& currentMin2);

		// todo: refactor GL object into another class
		unsigned int m_glVAO;
		unsigned int m_glVBO;
	};
} // namespace PotatoEngine

#endif // POTATO_ENGINE_POINT_CLOUD_MODEL_H_
