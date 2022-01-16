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

	float MeshModelBVH::GetElementCenter(int index, int dim)
	{
		auto& mesh = MeshModel::GetMesh();
		auto triangle = mesh.GetTriangle(index);

		float center = 0.f;
		for (int i = 0; i < 3; i++)
		{
			auto& vertex = mesh.GetVertex(triangle.v[i]);
			center += vertex.Position[dim];
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
		return QueryClosestPoint(GetRootNodeID(), queryPoint, maxSearchDist * maxSearchDist, currentMin2);
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
			for (uint32_t i = 0; i < elementCount; ++i)
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
