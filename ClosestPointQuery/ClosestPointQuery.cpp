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

#include "ClosestPointQuery.h"
#include "MeshModel.h"
#include "ModelCreator.h"
#include "Camera.h"
#include "ShaderObject/ShaderProgram.h"
#include "HelperDraw.h"
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
	this->Reset();
}

void ClosestPointQuery::ProcessInput()
{
	Game::ProcessInput();

	
}

void ClosestPointQuery::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
	{
		return;
	}

	// Press TAB to switch to console input
	// todo: somehow need to refactor a keyboard input module
	if (key == GLFW_KEY_TAB)
	{
		std::cout << "\nPlease input the queary point and max search distance: x y z distance\n";
		std::cin >> m_queryPoint.x >> m_queryPoint.y >> m_queryPoint.z >> m_maxSearchDistance;
		std::cout << "Input point and max distance: ("
			<< m_queryPoint.x << ", "
			<< m_queryPoint.y << ", "
			<< m_queryPoint.z << "), "
			<< m_maxSearchDistance << "\n";

		m_bToQuery = true;
	}

}

bool ClosestPointQuery::Init(const std::string& modelPath)
{
	// Base class Init() must be call before doing other initialization
	if (!Game::Init())
	{
		return false;
	}

	auto keyCallback = [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		static_cast<ClosestPointQuery*>(glfwGetWindowUserPointer(w))->KeyCallback(w, key, scancode, action, mods);
	};
	glfwSetKeyCallback(m_window, keyCallback);

	m_pShader = std::make_shared<ShaderProgram>();
	m_pShader->Create("GLSLSHaders/modelVertexShader.vs.glsl", "GLSLShaders/modelFragmentShader.fs.glsl");
	m_pShader->Use();

	m_pMeshModel = std::dynamic_pointer_cast<MeshModel>(ModelCreator::CreateModel(ModelCreator::Type::MESH_MODEL, modelPath));
	m_pMeshModel->SetColor(glm::vec3(0.8f));

#if 0
	// Adapth camera movement speed to the model size
	const auto& box = m_pModel->GetRoot()->box;
	auto diff = box.vmax - box.vmin;
	float maxDim = fmaxf(diff.x, fmaxf(diff.y, diff.z));
	m_pMainCamera->SetMoveSpeed(maxDim / 2.f);


	// Camera default look at direction (0, 0, -1), adapt camera position to the front of 
	// the bounding box front face
	auto midPoint = (box.vmax + box.vmin) / 2.f;
	m_pMainCamera->SetPosition(glm::vec3(midPoint.x, midPoint.y, box.vmax.z * 2.f));

#endif

	m_pQueryPointModel = std::dynamic_pointer_cast<SinglePointModel>(ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL));
	m_pQueryPointModel->SetColor(glm::vec4(1, 0, 0, 1)); // set query point red

	m_pClosestPointModel = std::dynamic_pointer_cast<SinglePointModel>(ModelCreator::CreateModel(ModelCreator::Type::SINGLE_POINT_MODEL));
	m_pClosestPointModel->SetColor(glm::vec4(0, 1, 0, 1)); // set result point green

	return true;
}

void ClosestPointQuery::Update()
{
	Game::Update();

	if (m_bToQuery)
	{
		m_bFoundResult = false;
		m_bToQuery = false;
		glm::vec3 closestPoint = DoQueryClosestPoint(m_queryPoint, m_maxSearchDistance);

		if (!isnan(closestPoint.x))
		{
			std::cout << "Closest point is: (" << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << ")" << std::endl;
			m_pClosestPointModel->SetPosition(closestPoint);

			m_bFoundResult = true;
		}

		m_pQueryPointModel->SetPosition(m_queryPoint);
	}
}


void ClosestPointQuery::Render()
{
	static const float aspect = (float)ScreenWidth() / (float)ScreenHeight();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = m_pMainCamera->GetViewingMatrix();
	glm::mat4 persp = m_pMainCamera->GetPerpectiveProjectionMatrix(aspect);

	m_pShader->Use();
	m_pShader->SetMat4("view", view);
	m_pShader->SetMat4("projection", persp);

	m_pMeshModel->Draw(m_pShader.get());

	DrawCoordAxis(m_pShader.get());

	m_pQueryPointModel->Draw(m_pShader.get());
	if (m_bFoundResult)
	{
		m_pClosestPointModel->Draw(m_pShader.get());
	}

}

void ClosestPointQuery::Reset()
{
	m_pShader->Release();

	Game::Reset();
}

int ClosestPointQuery::Run()
{
	std::cout << "Press TAB key to enter query point and search distance (you need to manually switch focus window to console).\n";
	return Game::Run();
}

glm::vec3 ClosestPointQuery::DoQueryClosestPoint(const glm::vec3& queryPoint, float maxSearchDistance)
{
	return QueryBruteForce(queryPoint, maxSearchDistance);
}

// Brute force method
glm::vec3 ClosestPointQuery::QueryBruteForce(const glm::vec3& queryPoint, float maxSearchDistance)
{
	auto& mesh = m_pMeshModel->GetMesh();
	const auto& indices = mesh.m_indices;
	const auto& vertices = mesh.m_vertices;

	float minDist2 = FLT_MAX;
	glm::vec3 res(NAN);
	for (std::size_t i = 0; i < indices.size(); i += 3)
	{
		auto& v0 = vertices[indices[i]].Position;
		auto& v1 = vertices[indices[i + 1]].Position;
		auto& v2 = vertices[indices[i + 2]].Position;
		glm::vec3 tres = ClosestPointOnTriangle(queryPoint, v0, v1, v2);
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
