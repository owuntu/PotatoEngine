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

using namespace PotatoEngine;

QueryClosestPoint::~QueryClosestPoint()
{

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
		std::cout << "\nPlease input the queary point and max distance: x y z distance\n";
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
			m_pClosestPointModel->SetColor(glm::vec4(0, 1, 0, 1)); // set green as result point

			m_bFoundResult = true;
		}

		m_pQueryPointModel->SetPosition(m_queryPoint);
		m_pQueryPointModel->SetColor(glm::vec4(1, 0, 0, 1)); // set red as query point
	}

	double sleepTime = 100.0;
	if (glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS)
	{
		maxKdTreeDrawDepth++;
		int maxDepth = m_pModel->GetMaxDepth();
		if (maxKdTreeDrawDepth > maxDepth)
		{
			maxKdTreeDrawDepth = maxDepth;
		}

		Sleep(sleepTime);
		std::cout << "\nDraw to Kd tree depth " << maxKdTreeDrawDepth << std::endl;
	}

	if (glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS)
	{
		maxKdTreeDrawDepth--;
		if (maxKdTreeDrawDepth <= -1)
		{
			maxKdTreeDrawDepth = -1;
		}
		Sleep(sleepTime);
		std::cout << "\nDraw to Kd tree depth " << maxKdTreeDrawDepth << std::endl;
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

	m_pModel = std::dynamic_pointer_cast<PointCloudModel>(ModelCreator::CreateModel(ModelCreator::Type::POINT_CLOUD_MODEL, "resources/objects/backpack/backpack.obj"));
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

static void DrawKdTree(ShaderProgram* pShader, const KdTree::Node* pNode, int depth)
{
	if (pNode == nullptr || pNode->splitAxis == -1 || depth > maxKdTreeDrawDepth)
	{
		return;
	}

	float planeSize = FLT_MAX;
	float pos = pNode->splitPos;

	glm::vec3 vertices[4] = {
		{pos, -planeSize, -planeSize},
		{pos,  planeSize, -planeSize},
		{pos,  planeSize,  planeSize},
		{pos, -planeSize,  planeSize}
	};

	for (int i = 0; i < 4; ++i)
	{
		std::swap(vertices[i][0], vertices[i][pNode->splitAxis]);
		
		for (int j = 0; j < 3; ++j)
		{
			float& r = vertices[i][j];
			r = fmaxf(r, pNode->box.vmin[j]);
			r = fminf(r, pNode->box.vmax[j]);
		}
	}

	float alpha = 0.5f;
	static glm::vec4 colors[3] = {
		{0.8f, 0, 0, alpha},
		{0, 0.8f, 0, alpha},
		{0, 0.8f, 0.8f, alpha}
	};

	pShader->SetMat4("modelMat", IDENTITY);
	pShader->SetVec4("ModelColor", colors[depth % 3]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(&(vertices[0][0]));
	glVertex3fv(&(vertices[1][0]));
	glVertex3fv(&(vertices[2][0]));
	glVertex3fv(&(vertices[3][0]));
	glEnd();

	DrawKdTree(pShader, pNode->left, depth + 1);
	DrawKdTree(pShader, pNode->right, depth + 1);
}

static void DrawCoordAxis(ShaderProgram* pShader)
{
	static float axisLenght = 10.f;
	static glm::vec3 axis[3] =
	{
		{axisLenght, 0, 0},
		{0, axisLenght, 0},
		{0, 0, axisLenght}
	};

	static glm::vec4 colors[3] = {
		{0.8f, 0, 0, 1.f},
		{0, 0.8f, 0, 1.f},
		{0, 0, 0.8f, 1.f}
	};

	pShader->SetMat4("modelMat", IDENTITY);
	for (int i = 0; i < 3; ++i)
	{
		pShader->SetVec4("ModelColor", glm::vec4(colors[i]));
		glBegin(GL_LINES);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3fv(&(axis[i][0]));
		glEnd();
	}
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
	DrawKdTree(m_pShader.get(), m_pModel->GetRoot(), 0);
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
	return QueryKDTree(queryPoint, maxSearchDistance);
}

// Basic method: brute force search
glm::vec3 QueryClosestPoint::QueryBruteForce(const glm::vec3& queryPoint, float maxSearchDistance)
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
glm::vec3 QueryClosestPoint::QueryKDTree(const glm::vec3& queryPoint, float maxSearchDistance)
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


