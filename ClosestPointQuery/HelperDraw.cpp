#include <glad/glad.h>
#include <glm/glm.hpp>

#include "HelperDraw.h"

using namespace PotatoEngine;

static const glm::mat4 IDENTITY = glm::mat4(1.0f);

void DrawKdTree(ShaderProgram* pShader, const KdTree::Node* pNode, int depth, const int maxDrawDepth)
{
	if (pNode == nullptr || pNode->splitAxis == -1 || depth > maxDrawDepth)
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

	DrawKdTree(pShader, pNode->left, depth + 1, maxDrawDepth);
	DrawKdTree(pShader, pNode->right, depth + 1, maxDrawDepth);
}

void DrawCoordAxis(ShaderProgram* pShader)
{
	static float axisLenght = 10.f;
	static glm::vec3 axis[3] =
	{
		{axisLenght, 0, 0},
		{0, axisLenght, 0},
		{0, 0, axisLenght}
	};

	static glm::vec4 colors[3] = {
		{.8f, .2f, .2f, 1.f},
		{.2f, .8f, .2f, 1.f},
		{.2f, .2f, .8f, 1.f}
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
