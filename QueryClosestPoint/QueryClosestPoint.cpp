#include <iostream>
#include <iomanip>

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include "QueryClosestPoint.h"

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
