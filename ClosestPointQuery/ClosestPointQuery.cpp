#include <algorithm>
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ClosestPointQuery.h"
#include "MeshModelBVH.h"
#include "BVHModelCreator.h"
#include "ClosestPointTest.h"

using namespace PotatoEngine;

static const glm::mat4 IDENTITY = glm::mat4(1.0f);

std::shared_ptr<ClosestPointQuery> ClosestPointQuery::Create(const std::string& modelPath)
{
	if (modelPath == "")
	{
		std::cout << "Warning: empty model path\n";
	}

	std::shared_ptr<ClosestPointQuery> pGame = std::make_shared<ClosestPointQuery>();
	if (!pGame->Init(modelPath))
	{
		return nullptr;
	}

	return pGame;
}

ClosestPointQuery::~ClosestPointQuery()
{
}

bool ClosestPointQuery::Init(const std::string& modelPath)
{
	BVHModelCreator bvhModelCreator;
	m_pMeshModel = std::dynamic_pointer_cast<MeshModelBVH>(bvhModelCreator.CreateModel(ModelCreator::Type::MESH_MODEL, modelPath));

	return true;
}

void ClosestPointQuery::Update()
{
	std::cout << "\nPlease input the queary point and max search distance: x y z distance\n";
	std::cin >> m_queryPoint.x >> m_queryPoint.y >> m_queryPoint.z >> m_maxSearchDistance;


	std::cout << "Input point and max distance: ("
		<< m_queryPoint.x << ", "
		<< m_queryPoint.y << ", "
		<< m_queryPoint.z << "), "
		<< m_maxSearchDistance << "\n";

	glm::vec3 closestPoint = DoQueryClosestPoint(m_queryPoint, m_maxSearchDistance);

	if (!glm::isnan(closestPoint.x))
	{
		std::cout << "Closest point is: (" << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << ")" << std::endl;
	}	
}

int ClosestPointQuery::Run()
{
	while (true)
	{
		Update();
	}

	return 0;
}

glm::vec3 ClosestPointQuery::DoQueryClosestPoint(const glm::vec3& queryPoint, float maxSearchDistance)
{
	//return QueryBruteForce(queryPoint, maxSearchDistance);
	return QueryBVH(queryPoint, maxSearchDistance);
}

// Brute force method
glm::vec3 ClosestPointQuery::QueryBruteForce(const glm::vec3& queryPoint, float maxSearchDistance)
{
	auto& mesh = m_pMeshModel->GetMesh();
	const auto numTriangles = mesh.NumTriangles();

	float minDist2 = FLT_MAX;
	glm::vec3 res(NAN);
	for (std::size_t i = 0; i < numTriangles; ++i)
	{
		glm::vec3 tres = mesh.ClosestPointOnTriangle(queryPoint, i);
		float d2 = glm::distance2(queryPoint, tres);
		if (d2 < minDist2)
		{
			minDist2 = d2;
			res = tres;
		}
	}

	if (minDist2 < maxSearchDistance * maxSearchDistance)
	{
		return res;
	}

	return glm::vec3(NAN);
}

glm::vec3 ClosestPointQuery::QueryBVH(const glm::vec3& queryPoint, float maxSearchDistance)
{
	return m_pMeshModel->QueryClosestPoint(queryPoint, maxSearchDistance);
}
