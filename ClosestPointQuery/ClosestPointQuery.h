#ifndef QUERY_CLOSEST_POINT_H_
#define QUERY_CLOSEST_POINT_H_
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace PotatoEngine
{
	class Model;
	class MeshModelBVH;
	class ShaderProgram;
} // namespace PotatoEngine

class ClosestPointQuery
{
public:
	virtual ~ClosestPointQuery();
	bool Init(const std::string& modelPath);
	int Run();

	// Method to query the closest point in m_pModel given a query point and max search distance.
	// Return NAN vec3 if such closest point cannot be found within the search distance.
	// Assume the input queryPoint and maxSearchDistance is always non-NAN
	glm::vec3 DoQueryClosestPoint(const glm::vec3& queryPoint, float maxSearchDistance);

	glm::vec3 QueryBruteForce(const glm::vec3& queryPoint, float maxSearchDistance);
	glm::vec3 QueryBVH(const glm::vec3& queryPoint, float maxSearchDistance);

	const std::shared_ptr<PotatoEngine::MeshModelBVH> GetModel() const { return m_pMeshModel; }

	static std::shared_ptr<ClosestPointQuery> Create(const std::string& modelPath);

protected:
	virtual void Update();

	std::shared_ptr<PotatoEngine::MeshModelBVH> m_pMeshModel;

private:
	glm::vec3 m_queryPoint;
	float m_maxSearchDistance;
};

#endif // QUERY_CLOSEST_POINT_H_

