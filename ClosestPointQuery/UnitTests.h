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
	{
		std::string testFileDir = "./";
		std::string modelFileName = modelPath.substr(modelPath.find_last_of("/\\") + 1);
		m_testDataFileName = testFileDir + modelFileName + "__test_data.txt";
		m_resultDataFileName = testFileDir + modelFileName + "__result_data.txt";
	}

	void GenerateTestPointsAndResults();
	void LoadTestData();

	void RunAllTests();

private:
	bool TestBVHSearch();
	bool TestBruteForceSearch();
	bool Verify(const std::vector<glm::vec3>& results);

	float m_maxSearchDistance ;
	int m_numPointsPerDim;

	std::string m_testDataFileName;
	std::string m_resultDataFileName;

	const std::string m_testModelPath;
};

#endif
