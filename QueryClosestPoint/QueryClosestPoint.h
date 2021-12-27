#ifndef QUERY_CLOSEST_POINT_H_
#define QUERY_CLOSEST_POINT_H_
#include <memory>
#include <glm/glm.hpp>
#include "Game.h"

namespace PotatoEngine
{
	class MeshModel;
	class Model;
	class ShaderProgram;
} // namespace PotatoEngine

class QueryClosestPoint : public PotatoEngine::Game
{
public:
	virtual ~QueryClosestPoint();
	virtual bool Init();
	virtual void Reset();
protected:
	virtual void Update();
	virtual void Render();

	virtual void ProcessInput();

	std::shared_ptr<PotatoEngine::Model> m_pModel;
	// todo: refactor shader into a renderer
	std::shared_ptr<PotatoEngine::ShaderProgram> m_pShader;
};

#endif // QUERY_CLOSEST_POINT_H_

