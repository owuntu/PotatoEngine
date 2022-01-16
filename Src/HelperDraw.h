#ifndef HELPER_DRAW_H_
#define HELPER_DRAW_H_

#include "KdTree.h"
#include "BVH.h"
#include "ShaderObject/ShaderProgram.h"

void DrawKdTree(PotatoEngine::ShaderProgram* pShader, const PotatoEngine::KdTree::Node* pNode, int depth, const int maxDrawDepth);
void DrawCoordAxis(PotatoEngine::ShaderProgram* pShader);

class PotatoEngine::BBox;

void DrawBox(const PotatoEngine::BBox& box);
void DrawBVH(PotatoEngine::ShaderProgram* pShader, const PotatoEngine::BVH::TempNode* pNode, int depth, int depthToDraw);

#endif // HELPER_DRAW_H_
