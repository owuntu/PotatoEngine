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

	void GenerateTestPointsAndResults();
	void LoadTestData();

	bool TestKDTreeSearch();
	bool TestBruteForceSearch();

private:
	bool Verify(const std::vector<glm::vec3>& results);

	float m_maxSearchDistance = 1.5f;
	int m_numPointsPerDim = 4;

	// Hard code file name
	const std::string m_testDataFileName = "QueryClosestPoint/testFile.txt";
	const std::string m_resultDataFileName = "QueryClosestPoint/resultFile.txt";
};

#endif
