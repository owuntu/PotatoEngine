#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/norm.hpp>

#include "BBox.h"
#include "PointCloudModel.h"
#include "ClosestPointQuery.h"
#include "UnitTests.h"

#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) x
#endif // _DEBUG

void WritePointsToFile(const std::vector<glm::vec3>& points, std::ofstream& testFile)
{
	for (auto point : points)
	{
		testFile << point.x << " " << point.y << " " << point.z << "\n";
	}
}

void WritePointsToFile(const std::vector<glm::vec3>& points, const std::string& fileName)
{
	std::ofstream testFile(fileName);
	if (!testFile.is_open())
	{
		std::string msg = "Cannot open file " + fileName;
		throw std::exception(msg.c_str());
	}

	WritePointsToFile(points, testFile);
	testFile.close();
}

void ReadFileToPoints(std::vector<glm::vec3>& points, std::ifstream& file)
{
	using namespace std;
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

	ReadFileToPoints(points, file);

	file.close();
}

// Basic method: brute force search
glm::vec3 QueryClosestPointBruteForce(const std::vector<glm::vec3>& points, const glm::vec3& queryPoint, float maxSearchDistance)
{
	glm::vec3 res(nanf(""));

	float minDist2 = FLT_MAX;
	float md2 = maxSearchDistance * maxSearchDistance;
	for (auto point : points)
	{
		float d2 = glm::length2(point - queryPoint);

		if (d2 < md2 && d2 < minDist2)
		{
			minDist2 = d2;
			res = point;
		}
	}

	return res;
}

void ClosestPointUnitTest::GenerateTestPointsAndResults()
{
	std::cout << "Generating test data and result files\n";
	using namespace PotatoEngine;
	auto sampleGame = ClosestPointQuery::Create(m_testModelPath);

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

	const auto& points = pModel->GetPoints();
	for (auto point : m_testPoints)
	{
		auto res = QueryClosestPointBruteForce(points, point, m_maxSearchDistance);
		m_expResults.push_back(res);
	}

	{
		std::ofstream testFile(m_testDataFileName);
		if (!testFile.is_open())
		{
			std::string msg = "Cannot open file " + m_testDataFileName;
			throw std::exception(msg.c_str());
		}

		testFile << m_maxSearchDistance << "\n";
		for (auto point : m_testPoints)
		{
			testFile << point.x << " " << point.y << " " << point.z << "\n";
		}
		testFile.close();
	}

	//WritePointsToFile(m_testPoints, m_testDataFileName);
	WritePointsToFile(m_expResults, m_resultDataFileName);
}


void ClosestPointUnitTest::LoadTestData()
{
	m_testPoints.clear();
	m_expResults.clear();

	{
		using namespace std;
		ifstream testFile(m_testDataFileName);
		if (!testFile.is_open())
		{
			std::string msg = "Cannot open file " + m_testDataFileName;
			throw std::exception(msg.c_str());
		}

		// The first line is the max search distance
		testFile >> m_maxSearchDistance;
		ReadFileToPoints(m_testPoints, testFile);
		testFile.close();
	}

	//ReadFileToPoints(m_testPoints, m_testDataFileName);
	ReadFileToPoints(m_expResults, m_resultDataFileName);
}

void ClosestPointUnitTest::RunAllTests()
{
	std::cout << "Running all tests\n";
	LoadTestData();
	// todo: may use test framework such as Google Test
	ASSERT(TestKDTreeSearch());
	ASSERT(TestBruteForceSearch());
}

bool ClosestPointUnitTest::TestKDTreeSearch()
{
	auto sampleGame = ClosestPointQuery::Create(m_testModelPath);

	std::vector<glm::vec3> results;

	using milli = std::chrono::milliseconds;
	auto start = std::chrono::high_resolution_clock::now();
	for (auto point : m_testPoints)
	{
		auto res = sampleGame->DoQueryClosestPoint(point, m_maxSearchDistance);
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
	auto sampleGame = ClosestPointQuery::Create(m_testModelPath);

	std::vector<glm::vec3> results;

	using milli = std::chrono::milliseconds;
	auto start = std::chrono::high_resolution_clock::now();
	auto pModel = sampleGame->GetModel();
	const auto& points = pModel->GetPoints();
	for (auto point : m_testPoints)
	{
		auto res = QueryClosestPointBruteForce(points, point, m_maxSearchDistance);
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
		std::cerr << "Number of result data doesn't match expected one.\n";
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
