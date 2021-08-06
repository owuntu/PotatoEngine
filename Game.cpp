#include <iostream>
#include <glfw3.h>

#include "Game.h"

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
	m_window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);
	return true;
}

int Game::Run()
{
	if (m_window == nullptr)
	{
		std::cerr << "GLFW has not been initialized yet" << std::endl;
		return 1;
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

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
}

} // namespace PotatoEngine
