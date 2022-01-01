#ifndef QUERY_CLOSEST_POINT_UNIT_TESTS_H_
#define QUERY_CLOSEST_POINT_UNIT_TESTS_H_

#include <vector>
#include <string>

#include <glm/glm.hpp>

class ClosestPointUnitTest
{
public:
	std::vector<glm::vec3> m_testPoints;
	std::vector<glm::vec3> m_expResults;

	ClosestPointUnitTest(const std::string& modelPath, float maxSearchDistance, int numPointsPerDim)
		: m_testModelPath(modelPath),
		m_maxSearchDistance(maxSearchDistance),
		m_numPointsPerDim(numPointsPerDim)
	{}

	void GenerateTestPointsAndResults();
	void LoadTestData();

	void RunAllTests();

private:
	bool TestKDTreeSearch();
	bool TestBruteForceSearch();
	bool Verify(const std::vector<glm::vec3>& results);

	float m_maxSearchDistance ;
	int m_numPointsPerDim;

	// Hard code file name
	const std::string m_testDataFileName = "QueryClosestPoint/testFile.txt";
	const std::string m_resultDataFileName = "QueryClosestPoint/resultFile.txt";

	const std::string m_testModelPath;
};

#endif
