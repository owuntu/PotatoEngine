#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>

#include "Model.h"
#include "Camera.h"
#include "ShaderObject/ShaderProgram.h"

#include "Game.h"

const static int gs_SCREEN_WIDTH = 1280;
const static int gs_SCREEN_HEIGHT = 720;

namespace PotatoEngine
{

bool Game::Init()
{
	/* Initialize the library */
	if (!glfwInit())
	{
		return false;
	}

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(gs_SCREEN_WIDTH, gs_SCREEN_HEIGHT, "Hello World", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	if (m_pMainCamera != nullptr)
	{
		delete m_pMainCamera;
		m_pMainCamera = nullptr;
	}

	m_pMainCamera = new Camera;

	m_lastFrameTime = glfwGetTime();

	return true;
}

int Game::Run()
{
	if (m_window == nullptr)
	{
		std::cerr << "GLFW has not been initialized yet" << std::endl;
		return 1;
	}

	ShaderProgram modelShader;
	modelShader.Create("GLSLSHaders/modelVertexShader.vs.glsl", "GLSLShaders/modelFragmentShader.fs.glsl");
	modelShader.Use();

	Model newModel("resources/objects/backpack/backpack.obj");

	glEnable(GL_DEPTH_TEST);

	const float aspect = (float)gs_SCREEN_WIDTH / (float)gs_SCREEN_HEIGHT;
	glm::mat4 identity = glm::mat4(1.0f);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		Update();
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = m_pMainCamera->GetViewingMatrix();
		glm::mat4 persp = m_pMainCamera->GetPerpectiveProjectionMatrix(aspect);

		modelShader.Use();
		modelShader.SetMat4("view", view);
		modelShader.SetMat4("projection", persp);
		modelShader.SetMat4("modelMat", identity);
		//newModel.Draw();
		newModel.DrawVertices();

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	modelShader.Release();

	glfwTerminate();

	return 0;
}

void Game::Reset()
{
	glfwDestroyWindow(m_window);
	m_window = nullptr;

	if (m_pMainCamera != nullptr)
	{
		delete m_pMainCamera;
		m_pMainCamera = nullptr;
	}
}

void Game::Update()
{
	float currentFrameTime = glfwGetTime();
	m_deltaTime = currentFrameTime - m_lastFrameTime;
	m_lastFrameTime = currentFrameTime;

	ProcessInput();
}

void Game::ProcessInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	ProcessKeyboardPress(GLFW_KEY_W);
	ProcessKeyboardPress(GLFW_KEY_S);
	ProcessKeyboardPress(GLFW_KEY_A);
	ProcessKeyboardPress(GLFW_KEY_D);
}

void Game::ProcessKeyboardPress(int key)
{
	Camera::Movement camMove;

	switch (key)
	{
	case GLFW_KEY_W:
		camMove = Camera::Movement::FORWARD;
		break;
	case GLFW_KEY_S:
		camMove = Camera::Movement::BACKWARD;
		break;
	case GLFW_KEY_A:
		camMove = Camera::Movement::LEFT;
		break;
	case GLFW_KEY_D:
		camMove = Camera::Movement::RIGHT;
		break;
	default:
		break;
	}

	if (glfwGetKey(m_window, key) == GLFW_PRESS)
	{
		m_pMainCamera->ProcessMovement(camMove, m_deltaTime);
	}
}

} // namespace PotatoEngine
