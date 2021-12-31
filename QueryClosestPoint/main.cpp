#include <cassert>
#include "QueryClosestPoint.h"
#include "UnitTests.h"

#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) x
#endif // _DEBUG

int main()
{
	using namespace PotatoEngine;
#ifdef UNIT_TESTS
	ClosestPointUnitTest test;
	//test.GenerateTestPointsAndResults();
	test.LoadTestData();
	ASSERT(test.TestKDTreeSearch());
	ASSERT(test.TestBruteForceSearch());
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