#ifndef QUERY_CLOSEST_POINT_H_
#define QUERY_CLOSEST_POINT_H_
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "Game.h"

namespace PotatoEngine
{
	class Model;
	class SinglePointModel;
	class PointCloudModel;
	class MeshModelBVH;
	class ShaderProgram;
} // namespace PotatoEngine

class ClosestPointQuery : public PotatoEngine::Game
{
public:
	virtual ~ClosestPointQuery();
	bool Init(const std::string& modelPath);
	virtual int Run();
	virtual void Reset();

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
	virtual void Render();

	virtual void ProcessInput();
	// todo: may want to refactor into another input class
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	std::shared_ptr<PotatoEngine::MeshModelBVH> m_pMeshModel;

	// todo: refactor shader into a renderer
	std::shared_ptr<PotatoEngine::ShaderProgram> m_pShader;

private:

	glm::vec3 m_queryPoint;
	float m_maxSearchDistance;

	std::shared_ptr<PotatoEngine::SinglePointModel> m_pQueryPointModel; // only for rendering for now
	std::shared_ptr<PotatoEngine::SinglePointModel> m_pClosestPointModel;

	bool m_bFoundResult = false;
	bool m_bToQuery = false;
};

#endif // QUERY_CLOSEST_POINT_H_

