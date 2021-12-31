#include "KdTree.h"

namespace PotatoEngine
{
	void KdTree::Build()
	{
		int numElements = GetTotalNumOfElements();
		std::vector<int> elements(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			elements[i] = i;
		}

		m_maxDepth = 0;
		m_root = BuildTree(elements, 0);
	}

	KdTree::Node* KdTree::BuildTree(std::vector<int> elements, int depth)
	{
		using namespace std;
		if (elements.empty())
		{
			return nullptr;
		}

		Node* res = new Node();

		m_maxDepth = (depth > m_maxDepth) ? depth : m_maxDepth;

		if (elements.size() <= MAX_NUM_NODE_ELEMENTS)
		{
			res->elements = elements;
			res->box = GetBoundingBox(elements);
			return res;
		}

		res->box.Init();
		int axis = depth % 3;
		Sort(elements, axis, res);

		{
			vector<int>::const_iterator first = elements.begin();
			vector<int>::const_iterator last = elements.begin() + elements.size() / 2;
			vector<int> leftArry(first, last);
			res->left = BuildTree(leftArry, depth + 1);
			if (res->left != nullptr)
			{
				res->box.Add(res->left->box);
			}
		}

		{
			vector<int>::const_iterator first = elements.begin() + elements.size() / 2;
			vector<int>::const_iterator last = elements.end();
			vector<int> rightArry(first, last);
			res->right = BuildTree(rightArry, depth + 1);
			if (res->right != nullptr)
			{
				res->box.Add(res->right->box);
			}
		}

		return res;
	}
} // namesapce PotatoEngine
