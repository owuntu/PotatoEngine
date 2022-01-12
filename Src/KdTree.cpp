#include <stack>

#include "KdTree.h"

namespace PotatoEngine
{
	const int KdTree::MAX_NUM_NODE_ELEMENTS = 32;

	void KdTree::Build()
	{
		int numElements = GetTotalNumOfElements();
		m_tmpElements.resize(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			m_tmpElements[i] = i;
		}

		m_maxDepth = 0;
		m_root = BuildTree(0, numElements, 0);
		//m_root = BuildTreeIteration();
		m_tmpElements.clear();
	}

	// A basic implementation of kd tree
	KdTree::Node* KdTree::BuildTree(int start, int end, int depth)
	{
		using namespace std;
		if (end <= start)
		{
			return nullptr;
		}

		Node* res = new Node();
		res->elements.clear();

		m_maxDepth = (depth > m_maxDepth) ? depth : m_maxDepth;

		int numElements = end - start;
		if (numElements <= MAX_NUM_NODE_ELEMENTS)
		{
			// Reach leaf node
			for (int i = start; i < end; ++i)
			{
				res->elements.push_back(m_tmpElements[i]);
			}
			res->box = GetBoundingBox(res->elements);
			return res;
		}

		res->box.Init();

		// Cycle through different axis
		int axis = GetSplitAxis(start, end, depth);
		Sort(start, end, axis, res);

		// Using the median point for split plane
		int median = (start + end) / 2;

		res->left = BuildTree(start, median, depth + 1);
		if (res->left != nullptr)
		{
			res->box.Add(res->left->box);
		}

		res->right = BuildTree(median, end, depth + 1);
		if (res->right != nullptr)
		{
			res->box.Add(res->right->box);
		}
		return res;
	}

} // namesapce PotatoEngine
