#include <iostream>
#include "QueryClosestPoint.h"
#include "UnitTests.h"


int main(int argc, const char** argv)
{
	using namespace PotatoEngine;
	if (argc < 2)
	{
		std::cout << "Usage: QueryClosestPoint [model path]\n";
		return 1;
	}

	std::string modelPath = argv[1];

#ifdef UNIT_TESTS
	ClosestPointUnitTest test(modelPath);
	// Uncomment bellow line to regenerate test files.
	//test.GenerateTestPointsAndResults();
	test.RunAllTests();
#else
	auto sampleGame = QueryClosestPoint::Create(modelPath);

	if (sampleGame == nullptr)
	{
		return 1;
	}

	sampleGame->Run();
#endif

	return 0;
}