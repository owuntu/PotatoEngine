#ifndef POTATOENGINE_POINTCLOUD_MODEL_KDTREE_H_
#define POTATOENGINE_POINTCLOUD_MODEL_KDTREE_H_

#include "PointCloudModel.h"
#include "KdTree.h"

namespace PotatoEngine
{
	class PointCloudModelKdTree : public PointCloudModel, public KdTree
	{
	public:
		glm::vec3 SearchNearest(const glm::vec3& queryPoint);
	protected:
		virtual void PostSetup();
	private:
		// KdTree method
		void Sort(int start, int end, int axis, Node* node);
		int GetTotalNumOfElements() const;
		BBox GetBoundingBox(const std::vector<int>& elements) const;
		int GetSplitAxis(int start, int end, int depth);

		int GetSplitAxisRange(int start, int end);
		int GetSplitAxisVariance(int start, int end);

		// Recursive version
		glm::vec3 SearchNearest(const glm::vec3& queryPoint, const Node* pNode, float& currentMin2) const;
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_POINTCLOUD_MODEL_KDTREE_H_
