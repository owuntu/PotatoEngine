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

void DrawBox(const BBox& box)
{
	const glm::vec3& vmin = box.vmin;
	const glm::vec3& vmax = box.vmax;

	float x[2] = { vmin[0], vmax[0] };
	float y[2] = { vmin[1], vmax[1] };
	float z[2] = { vmin[2], vmax[2] };

	const glm::vec3 v[8] =
	{
		{ x[0], y[0], z[0] }, // 0
		{ x[1], y[0], z[0] }, // 1
		{ x[1], y[0], z[1] }, // 2
		{ x[0], y[0], z[1] }, // 3

		{ x[0], y[1], z[0] }, // 4
		{ x[1], y[1], z[0] }, // 5
		{ x[1], y[1], z[1] }, // 6
		{ x[0], y[1], z[1] }, // 7
	};

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; ++i)
	{
		glVertex3fv(&(v[i][0]));
	}
	glVertex3fv(&(v[0][0]));

	for (int i = 0; i < 4; ++i)
	{
		glVertex3fv(&(v[i + 4][0]));
	}
	glVertex3fv(&(v[4][0]));
	glEnd();

	glBegin(GL_LINES);
	glVertex3fv(&(v[1][0]));
	glVertex3fv(&(v[5][0]));
	glEnd();

	glBegin(GL_LINES);
	glVertex3fv(&(v[2][0]));
	glVertex3fv(&(v[6][0]));
	glEnd();

	glBegin(GL_LINES);
	glVertex3fv(&(v[3][0]));
	glVertex3fv(&(v[7][0]));
	glEnd();
}

void DrawBVH(PotatoEngine::ShaderProgram* pShader, const PotatoEngine::BVH::TempNode* pNode, int depth, int depthToDraw)
{
	static glm::vec4 colors[3] = {
		{0.8f, 0, 0, 1.f},
		{0, 0.8f, 0, 1.f},
		{0, 0.8f, 0.8f, 1.f}
	};

	if (pNode == nullptr || depth > depthToDraw)
	{
		return;
	}

	if (depth == depthToDraw)
	{
		pShader->SetMat4("modelMat", IDENTITY);
		pShader->SetVec4("ModelColor", colors[depth % 3]);
		DrawBox(pNode->box);
	}

	DrawBVH(pShader, pNode->child1, depth + 1, depthToDraw);
	DrawBVH(pShader, pNode->child2, depth + 1, depthToDraw);
}

