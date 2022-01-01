#include <cassert>
#include "QueryClosestPoint.h"
#include "UnitTests.h"


int main()
{
	using namespace PotatoEngine;
#ifdef UNIT_TESTS
	ClosestPointUnitTest test;
	//test.GenerateTestPointsAndResults();
	test.RunAllTests();
#else
	QueryClosestPoint sampleGame;

	bool res = sampleGame.Init();
	if (!res)
	{
		return 1;
	}

	sampleGame.Run();

	sampleGame.Reset();
#endif

	return 0;
}