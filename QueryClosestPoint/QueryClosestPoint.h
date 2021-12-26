#ifndef QUERY_CLOSEST_POINT_H_
#define QUERY_CLOSEST_POINT_H_
#include "Game.h"

class QueryClosestPoint : public PotatoEngine::Game
{
public:
	virtual ~QueryClosestPoint();
protected:
	virtual void ProcessInput();
};

#endif // QUERY_CLOSEST_POINT_H_

