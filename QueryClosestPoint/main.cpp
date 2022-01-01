#include <cassert>
#include "QueryClosestPoint.h"
#include "UnitTests.h"


int main()
{
	using namespace PotatoEngine;
#ifdef UNIT_TESTS
	ClosestPointUnitTest test("resources/objects/backpack/backpack.obj");
	//test.GenerateTestPointsAndResults();
	test.RunAllTests();
#else
	auto sampleGame = QueryClosestPoint::Create("resources/objects/backpack/backpack.obj");

	if (sampleGame == nullptr)
	{
		return 1;
	}

	sampleGame->Run();
#endif

	return 0;
}