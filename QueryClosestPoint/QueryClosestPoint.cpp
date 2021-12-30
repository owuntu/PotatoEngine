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
		if (closestPoint.x != NAN);
		{
			std::cout << "Closest point is: (" << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << ")" << std::endl;
			m_pClosestPointModel->SetPosition(closestPoint);
			m_pClosestPointModel->SetColor(glm::vec3(0, 1, 0)); // set green as result point

			m_bFoundResult = true;
		}

		m_pQueryPointModel->SetPosition(m_queryPoint);
		m_pQueryPointModel->SetColor(glm::vec3(1, 0, 0)); // set red as query point
	}

	double sleepTime = 20.0;
	if (glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS)
	{
		maxKdTreeDrawDepth++;
		Sleep(sleepTime);
	}

	if (glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS)
	{
		maxKdTreeDrawDepth--;
		if (maxKdTreeDrawDepth <= -1)
		{
			maxKdTreeDrawDepth = -1;
		}
		Sleep(sleepTime);
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

	m_pQueryPointModel = std::dynamic_pointer_cast<SinglePointModel>(ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL));
	m_pClosestPointModel = std::dynamic_pointer_cast<SinglePointModel>(ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL));

	return true;
}

void QueryClosestPoint::Update()
{
	Game::Update();
}

void DrawKdTree(ShaderProgram* pShader, const KdTree::Node* pNode, int depth)
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

	static glm::vec3 colors[3] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 1, 1}
	};

	pShader->SetVec3("ModelColor", colors[depth % 3]);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(&(vertices[0][0]));
	glVertex3fv(&(vertices[1][0]));
	glVertex3fv(&(vertices[2][0]));
	glVertex3fv(&(vertices[3][0]));
	glEnd();

	DrawKdTree(pShader, pNode->left, depth + 1);
	DrawKdTree(pShader, pNode->right, depth + 1);
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

	m_pModel->Draw(m_pShader.get());
	DrawKdTree(m_pShader.get(), m_pModel->GetRoot(), 0);

	if (m_bFoundResult)
	{
		m_pQueryPointModel->Draw(m_pShader.get());
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
	//return QueryBruteForce(queryPoint, maxSearchDistance);
	return QueryKDTree(queryPoint, maxSearchDistance);
}

// Basic method: brute force search
glm::vec3 QueryClosestPoint::QueryBruteForce(const glm::vec3& queryPoint, float maxSearchDistance)
{
	using namespace PotatoEngine;
	glm::vec3 res(NAN, NAN, NAN);

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

// KdTree search
glm::vec3 QueryClosestPoint::QueryKDTree(const glm::vec3& queryPoint, float maxSearchDistance)
{
	using namespace PotatoEngine;
	glm::vec3 res(NAN, NAN, NAN);

	auto* tree = m_pModel->GetRoot();

	glm::vec3 tmp = m_pModel->SearchNearest(queryPoint);
	if (glm::distance2(tmp, queryPoint) > maxSearchDistance* maxSearchDistance)
	{
		return res;
	}
	res = tmp;

	return res;
}


