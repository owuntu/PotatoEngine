#include <iostream>
#include <iomanip>

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
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
		glm::vec3 point;
		float maxDist;
		std::cout << "\nPlease input the queary point and max distance: x y z distance\n";
		std::cin >> point.x >> point.y >> point.z >> maxDist;
		std::cout << "Input point and max distance: (" << point.x << ", " << point.y << ", " << point.z << "), " << maxDist << "\n";
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

	m_pModel->Draw();
}

void QueryClosestPoint::Reset()
{
	m_pShader->Release();

	Game::Reset();
}
