#include <iostream>
#include <iomanip>

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
	m_window = glfwCreateWindow(gs_SCREEN_WIDTH, gs_SCREEN_HEIGHT, "Potato Engine", NULL, NULL);
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

	m_mouseLastX = gs_SCREEN_WIDTH * 0.5f;
	m_mouseLastY = gs_SCREEN_HEIGHT * 0.5f;

	glfwSetWindowUserPointer(m_window, this);

	auto mouseCallback = [](GLFWwindow* w, double x, double y)
	{
		static_cast<Game*>(glfwGetWindowUserPointer(w))->MouseCallBack(w, x, y);
	};

	glfwSetCursorPosCallback(m_window, mouseCallback);

	m_pMainCamera.reset();

	m_pMainCamera = std::make_shared<Camera>();

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

	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		Update();

		static float outputInterval = 0.0f;
		if (outputInterval > 1.0f)
		{
			float fps = 1.0f / m_deltaTime;
			std::cout << "\rFPS: " << std::fixed << std::setprecision(2) << fps;
			outputInterval = 0.0f;
		}
		outputInterval += m_deltaTime;

		Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void Game::Reset()
{
	glfwDestroyWindow(m_window);
	m_window = nullptr;

	m_pMainCamera.reset();
}

int Game::ScreenWidth()
{
	return gs_SCREEN_WIDTH;
}

int Game::ScreenHeight()
{
	return gs_SCREEN_HEIGHT;
}

Game::~Game()
{
	this->Reset();
}

void Game::Update()
{
	float currentFrameTime = glfwGetTime();
	m_deltaTime = currentFrameTime - m_lastFrameTime;
	m_lastFrameTime = currentFrameTime;

	ProcessInput();
}

void Game::Render()
{
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
		// Not supported keyboard input
		return;
	}

	if (glfwGetKey(m_window, key) == GLFW_PRESS)
	{
		m_pMainCamera->ProcessMovement(camMove, m_deltaTime);
	}
}

void Game::MouseCallBack(GLFWwindow* window, double xPos, double yPos)
{
	static bool bFirstMouse = true;
	if (bFirstMouse)
	{
		m_mouseLastX = xPos;
		m_mouseLastY = yPos;
		bFirstMouse = false;
	}

	float xOffset = xPos - m_mouseLastX;
	float yOffset = m_mouseLastY - yPos; // reverse y since y-coordinate goes from bottom to top

	m_mouseLastX = xPos;
	m_mouseLastY = yPos;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		m_pMainCamera->ProcessMouseRotation(xOffset, yOffset);
	}

}

} // namespace PotatoEngine
