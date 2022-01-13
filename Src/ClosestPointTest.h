#ifndef POTATOENGINE_CLOSEST_POINT_TEST_H_
#define POTATOENGINE_CLOSEST_POINT_TEST_H_

#include <glm/glm.hpp>

namespace PotatoEngine
{
	glm::vec3 ClosestPointOnTriangle(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
} // namespace PotatoEngine

#endif
