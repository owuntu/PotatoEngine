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
	return true;
}

int Game::Run()
{
	if (m_window == nullptr)
	{
		std::cerr << "GLFW has not been initialized yet" << std::endl;
		return 1;
	}

#if 1
	ShaderProgram sProgram;
	sProgram.Create("GLSLShaders/vertexShader.vs.glsl","GLSLShaders/fragmentShader.fs.glsl");
#endif
	ShaderProgram modelShader;
	modelShader.Create("GLSLSHaders/modelVertexShader.vs.glsl", "GLSLShaders/modelFragmentShader.fs.glsl");
	modelShader.Use();

	Camera camera0;
	Model newModel("resources/objects/backpack/backpack.obj");

	glEnable(GL_DEPTH_TEST);
#if 1

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	{
		float vertices[] = {
			// positions         // colors
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}
	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
#endif

	const float aspect = (float)gs_SCREEN_WIDTH / (float)gs_SCREEN_HEIGHT;
	glm::mat4 identity = glm::mat4(1.0f);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera0.GetViewingMatrix();
		glm::mat4 persp = camera0.GetPerpectiveProjectionMatrix(aspect);

		sProgram.Use();
		sProgram.SetMat4("view", view);
		sProgram.SetMat4("projection", persp);
		sProgram.SetMat4("modelMat", identity);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		modelShader.Use();
		modelShader.SetMat4("view", view);
		modelShader.SetMat4("projection", persp);
		modelShader.SetMat4("modelMat", identity);
		newModel.Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	sProgram.Release();
	modelShader.Release();

	glfwTerminate();

	return 0;
}

void Game::Reset()
{
	glfwDestroyWindow(m_window);
	m_window = nullptr;
}

} // namespace PotatoEngine
