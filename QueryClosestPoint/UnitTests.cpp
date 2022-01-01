#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/epsilon.hpp>

#include "BBox.h"
#include "PointCloudModel.h"
#include "QueryClosestPoint.h"
#include "UnitTests.h"

#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) x
#endif // _DEBUG

bool WritePointsToFile(const std::vector<glm::vec3>& points, const std::string& fileName)
{
	std::ofstream testFile(fileName);
	if (!testFile.is_open())
	{
		std::string msg = "Cannot open file " + fileName;
		throw std::exception(msg.c_str());
	}

	for (auto point : points)
	{
		testFile << point.x << " " << point.y << " " << point.z << "\n";
	}
	testFile.close();
}

void ClosestPointUnitTest::GenerateTestPointsAndResults()
{
	using namespace PotatoEngine;
	auto sampleGame = QueryClosestPoint::Create(m_testModelPath);

	const std::shared_ptr<PointCloudModel> pModel = sampleGame->GetModel();
	BBox box = pModel->GetRoot()->box;

	// scale up the bounding box for larger area test
	box.vmax *= 2.0f;
	box.vmin *= 2.0f;

	m_testPoints.clear();
	m_expResults.clear();

	float zStep = (box.vmax.z - box.vmin.z) / (float)m_numPointsPerDim;
	float yStep = (box.vmax.y - box.vmin.y) / (float)m_numPointsPerDim;
	float xStep = (box.vmax.x - box.vmin.x) / (float)m_numPointsPerDim;
	for (int iz = 0; iz < m_numPointsPerDim; ++iz)
	{
		float z = box.vmin.z + (float)iz * zStep;
		for (int iy = 0; iy < m_numPointsPerDim; ++iy)
		{
			float y = box.vmin.y + (float)iy * yStep;
			for (int ix = 0; ix < m_numPointsPerDim; ++ix)
			{
				float x = box.vmin.x + (float)ix * xStep;
				m_testPoints.push_back(glm::vec3(x, y, z));
			}
		}
	}

	for (auto point : m_testPoints)
	{
		auto res = sampleGame->QueryClosestPointBruteForce(point, m_maxSearchDistance);
		m_expResults.push_back(res);
	}

	WritePointsToFile(m_testPoints, m_testDataFileName);
	WritePointsToFile(m_expResults, m_resultDataFileName);
}

void ReadFileToPoints(std::vector<glm::vec3>& points, const std::string& fileName)
{
	using namespace std;
	ifstream file(fileName);

	if (!file.is_open())
	{
		string msg = "Cannot open file " + fileName;
		throw std::exception(msg.c_str());
	}

	while (!file.eof())
	{
		string line = "";
		getline(file, line);
		if (line.size() <= 1)
		{
			continue;
		}

		istringstream s(line);
		glm::vec3 point;
		for (int axis = 0; axis < 3; ++axis)
		{
			if (!(s >> point[axis]))
			{
				point.x = point.y = point.z = NAN;
				break;
			}
		}
		points.push_back(point);
	}

	file.close();
}

void ClosestPointUnitTest::LoadTestData()
{
	m_testPoints.clear();
	m_expResults.clear();
	ReadFileToPoints(m_testPoints, m_testDataFileName);
	ReadFileToPoints(m_expResults, m_resultDataFileName);
}

void ClosestPointUnitTest::RunAllTests()
{
	LoadTestData();
	// todo: may use test framework such as Google Test
	ASSERT(TestKDTreeSearch());
	ASSERT(TestBruteForceSearch());
}

bool ClosestPointUnitTest::TestKDTreeSearch()
{
	auto sampleGame = QueryClosestPoint::Create(m_testModelPath);

	std::vector<glm::vec3> results;

	using milli = std::chrono::milliseconds;
	auto start = std::chrono::high_resolution_clock::now();
	for (auto point : m_testPoints)
	{
		auto res = sampleGame->QueryClosestPointKDTree(point, m_maxSearchDistance);
		results.push_back(res);
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "All KD Tree search took "
		<< std::chrono::duration_cast<milli>(finish - start).count()
		<< " milliseconds\n";

	bool bPassed = Verify(results);

	return bPassed;
}

bool ClosestPointUnitTest::TestBruteForceSearch()
{
	auto sampleGame = QueryClosestPoint::Create(m_testModelPath);

	std::vector<glm::vec3> results;

	using milli = std::chrono::milliseconds;
	auto start = std::chrono::high_resolution_clock::now();
	for (auto point : m_testPoints)
	{
		auto res = sampleGame->QueryClosestPointBruteForce(point, m_maxSearchDistance);
		results.push_back(res);
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "All brute force search took "
		<< std::chrono::duration_cast<milli>(finish - start).count()
		<< " milliseconds\n";

	bool bPassed = Verify(results);
	return bPassed;
}

bool ClosestPointUnitTest::Verify(const std::vector<glm::vec3>& results)
{
	bool bSuccess = true;
	if (results.size() != m_expResults.size())
	{
		return false;
	}

	for (int i = 0; i < m_expResults.size(); ++i)
	{
		auto exp = m_expResults[i];
		auto res = results[i];
		float epsilon = 0.01f;
		if ((isnan(exp.x) ^ isnan(res.x))
			|| glm::any(glm::epsilonNotEqual(exp, res, epsilon)))
		{
			bSuccess = false;
			std::cout << "Incorrect result at " << i << std::endl;
			std::cout << "Result: (" << res.x << ", " << res.y << ", " << res.z << ")\n";
			std::cout << "Expected: (" << exp.x << ", " << exp.y << ", " << exp.z << ")\n";
			break;
		}
	}

	return bSuccess;
}
