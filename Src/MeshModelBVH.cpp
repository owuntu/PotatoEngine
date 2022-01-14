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
		MeshModelBVH::PostSetup();
		BVH::Build(MeshModel::GetMesh().NumTriangles());
	}
} // namespace PotatoEngine
