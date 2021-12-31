#include "KdTree.h"

namespace PotatoEngine
{
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
		m_tmpElements.clear();
	}

	KdTree::Node* KdTree::BuildTree(int start, int end, int depth)
	{
		using namespace std;
		if (end <= start)
		{
			return nullptr;
		}

		Node* res = new Node();

		m_maxDepth = (depth > m_maxDepth) ? depth : m_maxDepth;
		int numElements = end - start;
		if (numElements <= MAX_NUM_NODE_ELEMENTS)
		{
			for (int i = start; i < end; ++i)
			{
				res->elements.push_back(m_tmpElements[i]);
			}
			res->box = GetBoundingBox(res->elements);
			return res;
		}

		res->box.Init();
		int axis = depth % 3;
		Sort(start, end, axis, res);

		int midPoint = (start + end) / 2;

		res->left = BuildTree(start, midPoint, depth + 1);
		if (res->left != nullptr)
		{
			res->box.Add(res->left->box);
		}

		res->right = BuildTree(midPoint, end, depth + 1);
		if (res->right != nullptr)
		{
			res->box.Add(res->right->box);
		}
		return res;
	}
} // namesapce PotatoEngine
