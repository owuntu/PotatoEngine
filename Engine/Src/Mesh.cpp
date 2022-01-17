#include "ClosestPointTest.h"

#include "Mesh.h"

namespace PotatoEngine
{
	Mesh::~Mesh()
	{
	}

	glm::vec3 Mesh::ClosestPointOnTriangle(const glm::vec3& p, std::size_t triangleIndex) const
	{
		const auto triangle = GetTriangle(triangleIndex);
		const auto& v0 = m_vertices[triangle.v[0]].Position;
		const auto& v1 = m_vertices[triangle.v[1]].Position;
		const auto& v2 = m_vertices[triangle.v[2]].Position;

		return PotatoEngine::ClosestPointOnTriangle(p, v0, v1, v2);
	}

} // namespace PotatoEngine
