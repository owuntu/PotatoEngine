#include <algorithm>
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "QueryClosestPoint.h"
#include "MeshModel.h"
#include "ModelCreator.h"
#include "Camera.h"
#include "ShaderObject/ShaderProgram.h"
#include "HelperDraw.h"

using namespace PotatoEngine;

std::shared_ptr<QueryClosestPoint> QueryClosestPoint::Create(const std::string& modelPath)
{
	if (modelPath == "")
	{
		std::cout << "Warning: empty model path\n";
	}

	std::shared_ptr<QueryClosestPoint> pGame = std::make_shared<QueryClosestPoint>();
	if (!pGame->Init(modelPath))
	{
		return nullptr;
	}

	return pGame;
}

QueryClosestPoint::~QueryClosestPoint()
{
	this->Reset();
}

static const glm::mat4 IDENTITY = glm::mat4(1.0f);
int maxKdTreeDrawDepth = 16;

void QueryClosestPoint::ProcessInput()
{
	Game::ProcessInput();

	// Press TAB to switch to console input
	// todo: somehow need to refactor a keyboard input module
	if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		m_bFoundResult = false;
		std::cout << "\nPlease input the queary point and max search distance: x y z distance\n";
		std::cin >> m_queryPoint.x >> m_queryPoint.y >> m_queryPoint.z >> m_maxSearchDistance;
		std::cout << "Input point and max distance: ("
			<< m_queryPoint.x << ", "
			<< m_queryPoint.y << ", "
			<< m_queryPoint.z << "), "
			<< m_maxSearchDistance << "\n";

		glm::vec3 closestPoint = DoQueryClosestPoint(m_queryPoint, m_maxSearchDistance);

		if (!isnan(closestPoint.x))
		{
			std::cout << "Closest point is: (" << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << ")" << std::endl;
			m_pClosestPointModel->SetPosition(closestPoint);
			m_pClosestPointModel->SetColor(glm::vec4(0, 1, 0, 1)); // set result point green

			m_bFoundResult = true;
		}

		m_pQueryPointModel->SetPosition(m_queryPoint);
		m_pQueryPointModel->SetColor(glm::vec4(1, 0, 0, 1)); // set query point red
	}

	double sleepTime = 100.0;
	bool bIPress = (glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS);
	bool bKPress = (glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS);
	if (bIPress || bKPress)
	{
		if (bIPress)
		{
			maxKdTreeDrawDepth++;
		}
		if (bKPress)
		{
			maxKdTreeDrawDepth--;
		}
		
		int maxDepth = m_pModel->GetMaxDepth();
		maxKdTreeDrawDepth = std::max(maxKdTreeDrawDepth, maxDepth);
		maxKdTreeDrawDepth = std::clamp(maxKdTreeDrawDepth, -1, maxDepth);
		Sleep(sleepTime);
		std::cout << "\nDraw to Kd tree depth " << maxKdTreeDrawDepth << std::endl;
	}
}

bool QueryClosestPoint::Init(const std::string& modelPath)
{
	// Base class Init() must be call before doing other initialization
	if (!Game::Init())
	{
		return false;
	}

	m_pShader = std::make_shared<ShaderProgram>();
	m_pShader->Create("GLSLSHaders/modelVertexShader.vs.glsl", "GLSLShaders/modelFragmentShader.fs.glsl");
	m_pShader->Use();

	m_pModel = std::dynamic_pointer_cast<PointCloudModel>(ModelCreator::CreateModel(ModelCreator::Type::POINT_CLOUD_MODEL, modelPath));
	m_pModel->SetColor(glm::vec3(0.8f));

	maxKdTreeDrawDepth = m_pModel->GetMaxDepth();

	m_pQueryPointModel = std::dynamic_pointer_cast<SinglePointModel>(ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL));
	m_pClosestPointModel = std::dynamic_pointer_cast<SinglePointModel>(ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL));

	return true;
}

void QueryClosestPoint::Update()
{
	Game::Update();
}


void QueryClosestPoint::Render()
{
	static const float aspect = (float)ScreenWidth() / (float)ScreenHeight();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = m_pMainCamera->GetViewingMatrix();
	glm::mat4 persp = m_pMainCamera->GetPerpectiveProjectionMatrix(aspect);

	m_pShader->Use();
	m_pShader->SetMat4("view", view);
	m_pShader->SetMat4("projection", persp);

	m_pModel->Draw(m_pShader.get());
	DrawKdTree(m_pShader.get(), m_pModel->GetRoot(), 0, maxKdTreeDrawDepth);
	DrawCoordAxis(m_pShader.get());

	m_pQueryPointModel->Draw(m_pShader.get());
	if (m_bFoundResult)
	{
		m_pClosestPointModel->Draw(m_pShader.get());
	}

}

void QueryClosestPoint::Reset()
{
	m_pShader->Release();

	Game::Reset();
}

glm::vec3 QueryClosestPoint::DoQueryClosestPoint(const glm::vec3& queryPoint, float maxSearchDistance)
{
	return QueryClosestPointKDTree(queryPoint, maxSearchDistance);
}

// Basic method: brute force search
glm::vec3 QueryClosestPoint::QueryClosestPointBruteForce(const glm::vec3& queryPoint, float maxSearchDistance)
{
	using namespace PotatoEngine;
	glm::vec3 res(nanf(""));

	const auto& points = reinterpret_cast<PointCloudModel*>(m_pModel.get())->GetPoints();
	float minDist2 = FLT_MAX;
	float md2 = maxSearchDistance * maxSearchDistance;
	for (auto point : points)
	{
		float d2 = glm::length2(point - queryPoint);

		if (d2 < md2 && d2 < minDist2)
		{
			minDist2 = d2;
			res = point;
		}
	}

	return res;
}

// KdTree search
glm::vec3 QueryClosestPoint::QueryClosestPointKDTree(const glm::vec3& queryPoint, float maxSearchDistance)
{
	using namespace PotatoEngine;
	glm::vec3 res(nanf(""));

	auto* tree = m_pModel->GetRoot();

	glm::vec3 tmp = m_pModel->SearchNearest(queryPoint);
	if (glm::distance2(tmp, queryPoint) > maxSearchDistance* maxSearchDistance)
	{
		return res;
	}
	res = tmp;

	return res;
}


