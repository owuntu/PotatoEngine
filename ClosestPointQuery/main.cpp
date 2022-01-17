#include <iostream>
#include "ClosestPointQuery.h"
#include "UnitTests.h"


int main(int argc, const char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: ClosestPointQuery [model path]\n";
		return 1;
	}

	std::string modelPath = argv[1];

#ifdef UNIT_TESTS
	ClosestPointUnitTest test(modelPath, 1.5f, 10);
	// Uncomment bellow line to regenerate test files.
	//test.GenerateTestPointsAndResults();
	test.RunAllTests();
	return 0;
#else
	using namespace PotatoEngine;
	auto sampleGame = ClosestPointQuery::Create(modelPath);

	if (sampleGame == nullptr)
	{
		return 1;
	}
	return sampleGame->Run();
#endif

}