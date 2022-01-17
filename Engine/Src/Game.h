#pragma once

#include <memory>

namespace PotatoEngine
{
class Camera;

class Game
{
public:
	virtual bool Init();
	virtual int Run();
	virtual void Reset();

	virtual ~Game();
protected:
	virtual void Update();
};
} // PotatoEngine
