#include <iostream>
#include <iomanip>

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "QueryClosestPoint.h"
#include "MeshModel.h"
#include "ModelCreator.h"
#include "Camera.h"
#include "ShaderObject/ShaderProgram.h"

using namespace PotatoEngine;

QueryClosestPoint::~QueryClosestPoint()
{

}

void QueryClosestPoint::ProcessInput()
{
	Game::ProcessInput();
	// Press TAB to switch to console input
	// todo: somehow need to refactor a keyboard input module
	if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		std::cout << "\nPlease input the queary point and max distance: x y z distance\n";
		std::cin >> m_queryPoint.x >> m_queryPoint.y >> m_queryPoint.z >> m_maxSearchDistance;
		std::cout << "Input point and max distance: ("
			<< m_queryPoint.x << ", "
			<< m_queryPoint.y << ", "
			<< m_queryPoint.z << "), "
			<< m_maxSearchDistance << "\n";

		glm::vec3 closestPoint = DoQueryClosestPoint(m_queryPoint, m_maxSearchDistance);
		if (closestPoint.x != NAN);
		{
			std::cout << "Closest point is: (" << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << ")" << std::endl;
			auto pClosestPoint = std::dynamic_pointer_cast<SinglePointModel>(m_pClosestPointModel);
			pClosestPoint->GetPoint() = closestPoint;
		}

		auto pQueryPoint = std::dynamic_pointer_cast<SinglePointModel>(m_pQueryPointModel);

		pQueryPoint->GetPoint() = m_queryPoint;
	}
}

bool QueryClosestPoint::Init()
{
	if (!Game::Init())
	{
		return false;
	}

	m_pShader = std::make_shared<ShaderProgram>();
	m_pShader->Create("GLSLSHaders/modelVertexShader.vs.glsl", "GLSLShaders/modelFragmentShader.fs.glsl");
	m_pShader->Use();

	m_pModel = ModelCreator::CreateModel(ModelCreator::Type::POINT_CLOUD_MODEL, "resources/objects/backpack/backpack.obj");

	m_pQueryPointModel = ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL);
	m_pClosestPointModel = ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL);

	return true;
}

void QueryClosestPoint::Update()
{
	Game::Update();
}

void QueryClosestPoint::Render()
{
	static const float aspect = (float)ScreenWidth() / (float)ScreenHeight();
	static const glm::mat4 identity = glm::mat4(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = m_pMainCamera->GetViewingMatrix();
	glm::mat4 persp = m_pMainCamera->GetPerpectiveProjectionMatrix(aspect);

	m_pShader->Use();
	m_pShader->SetMat4("view", view);
	m_pShader->SetMat4("projection", persp);
	m_pShader->SetMat4("modelMat", identity);

	m_pShader->SetVec3("ModelColor", glm::vec3(1, 1, 1));
	m_pModel->Draw();

	m_pShader->SetVec3("ModelColor", glm::vec3(1, 0, 0));
	m_pQueryPointModel->Draw();

	m_pShader->SetVec3("ModelColor", glm::vec3(0, 1, 0));
	m_pClosestPointModel->Draw();
}

void QueryClosestPoint::Reset()
{
	m_pShader->Release();

	Game::Reset();
}

glm::vec3 QueryClosestPoint::DoQueryClosestPoint(const glm::vec3& queryPoint, float maxSearchDistance)
{
	using namespace PotatoEngine;
	glm::vec3 res(NAN, NAN, NAN);

	// Basic method: brute force search
	const auto& points = reinterpret_cast<PointCloudModel*>(m_pModel.get())->GetPoints();
	float minDist = FLT_MAX;
	for (auto point : points)
	{
		float d = glm::length2(point - queryPoint);

		if (d < maxSearchDistance && d < minDist)
		{
			minDist = d;
			res = point;
		}
	}

	return res;
}
