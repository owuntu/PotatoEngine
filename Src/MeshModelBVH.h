#ifndef POTATOENGINE_MESH_MODEL_BVH_H_
#define POTATOENGINE_MESH_MODEL_BVH_H_

#include "MeshModel.h"
#include "BVH.h"

namespace PotatoEngine
{
	class MeshModelBVH : public MeshModel, public BVH
	{
	public:
		virtual ~MeshModelBVH() {}

		glm::vec3 QueryClosestPoint(const glm::vec3& queryPoint, float maxSearchDist = FLT_MAX) const;

	protected:

		virtual void GetElementBound(int index, BBox& box);
		virtual float GetElementCenter(int index, int dim);

	private:
		virtual void PostSetup();

		glm::vec3 QueryClosestPoint(const Node* pNode, const glm::vec3& queryPoint, const float searchDist2, float& currentMin2) const;

		glm::vec3 QueryClosestPoint(std::size_t nodeID, const glm::vec3& queryPoint, const float searchDist2, float& currentMin2) const;

	}; // class MeshModelBVH

} // namespace PotatoEngine


#endif // POTATOENGINE_MESH_MODEL_BVH_H_
