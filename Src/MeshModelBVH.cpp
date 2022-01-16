#include <glm/gtx/norm.hpp>
#include "ClosestPointTest.h"

#include "MeshModelBVH.h"

namespace PotatoEngine
{
	void MeshModelBVH::GetElementBound(int index, BBox& box)
	{
		auto& mesh = MeshModel::GetMesh();
		auto triangle = mesh.GetTriangle(index);

		for (int i = 0; i < 3; i++)
		{
			auto& vertex = mesh.GetVertex(triangle.v[i]);
			box += vertex.Position;
		}
	}

	glm::vec3 MeshModelBVH::GetElementCenter(int index)
	{
		auto& mesh = MeshModel::GetMesh();
		auto triangle = mesh.GetTriangle(index);

		glm::vec3 center(0.f);
		for (int i = 0; i < 3; i++)
		{
			auto& vertex = mesh.GetVertex(triangle.v[i]);
			center += vertex.Position;
		}

		center /= 3.f;
		return center;
	}

	void MeshModelBVH::PostSetup()
	{
		MeshModel::PostSetup();
		BVH::Build(MeshModel::GetMesh().NumTriangles());
	}


	glm::vec3 MeshModelBVH::QueryClosestPoint(const glm::vec3& queryPoint, float maxSearchDist) const
	{
		// todo: what if maxSearchDist > sqrt(FLT_MAX)?
		float currentMin2 = FLT_MAX;
		//return QueryClosestPoint(GetRoot(), queryPoint, maxSearchDist * maxSearchDist, currentMin2);
		return QueryClosestPoint(GetRootNodeID(), queryPoint, maxSearchDist * maxSearchDist, currentMin2);
	}

	glm::vec3 MeshModelBVH::QueryClosestPoint(const Node* pNode, const glm::vec3& queryPoint, const float searchDist2, float& currentMin2) const
	{
		if (pNode == nullptr)
		{
			return glm::vec3(NAN);
		}

		const auto& box = pNode->box;
		glm::vec3 boxClosest = ClosestPointOnAABB(queryPoint, box.vmin, box.vmax);

		float dist2 = glm::distance2(queryPoint, boxClosest);
		if (dist2 >= currentMin2 || dist2 > searchDist2)
		{
			// Early exit if distance from the closest point on AABB is
			// further than the search distance or currentMin
			return glm::vec3(NAN);
		}

		if (pNode->child1 == nullptr)
		{
			// on leaf node, find closest point on triangle
			glm::vec3 res(NAN);
			auto& mesh = GetMesh();
			for (unsigned int i = 0; i < pNode->numElements; ++i)
			{
				auto index = m_elements[i + pNode->elementOffset];
				
				glm::vec3 tres = mesh.ClosestPointOnTriangle(queryPoint, index);
				dist2 = glm::distance2(queryPoint, tres);
				if (dist2 <= searchDist2 && dist2 < currentMin2)
				{
					currentMin2 = dist2;
					res = tres;
				}
			}
			return res;
		}

		auto res1 = QueryClosestPoint(pNode->child1, queryPoint, searchDist2, currentMin2);
		auto res2 = QueryClosestPoint(pNode->child2, queryPoint, searchDist2, currentMin2);

		if (isnan(res2.x))
		{
			return res1;
		}

		return res2;
	}
	
	glm::vec3 MeshModelBVH::QueryClosestPoint(std::size_t nodeID, const glm::vec3& queryPoint, const float searchDist2, float& currentMin2) const
	{
		const auto& node = m_nodes[nodeID];
		const auto& box = node.GetBoundingBox();
		glm::vec3 boxClosest = ClosestPointOnAABB(queryPoint, box.vmin, box.vmax);

		float dist2 = glm::distance2(queryPoint, boxClosest);
		if (dist2 >= currentMin2 || dist2 > searchDist2)
		{
			// Early exit if distance from the closest point on AABB is
			// further than the search distance or currentMin
			return glm::vec3(NAN);
		}

		if (node.IsLeafNode())
		{
			// on leaf node, find closest point on triangle
			glm::vec3 res(NAN);
			auto& mesh = GetMesh();
			auto elementCount = node.GetElementCount();
			auto elementOffset = node.GetElementOffset();
			for (unsigned int i = 0; i < elementCount; ++i)
			{
				auto index = m_elements[i + elementOffset];

				glm::vec3 tres = mesh.ClosestPointOnTriangle(queryPoint, index);
				dist2 = glm::distance2(queryPoint, tres);
				if (dist2 <= searchDist2 && dist2 < currentMin2)
				{
					currentMin2 = dist2;
					res = tres;
				}
			}
			return res;
		}

		auto res1 = QueryClosestPoint(node.GetChild1Index(), queryPoint, searchDist2, currentMin2);
		auto res2 = QueryClosestPoint(node.GetChild2Index(), queryPoint, searchDist2, currentMin2);

		if (isnan(res2.x))
		{
			return res1;
		}

		return res2;
	}

} // namespace PotatoEngine
