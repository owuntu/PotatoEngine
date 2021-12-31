#include <stack>

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
		//m_root = BuildTreeIteration();
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

	// todo: Need to be fixed. There is overlap split plane using iteration.
	KdTree::Node* KdTree::BuildTreeIteration()
	{
		using namespace std;
		if (m_tmpElements.empty())
		{
			return nullptr;
		}

		m_root = new Node();
		m_root->box.Init();

		struct TmpNode
		{
			Node* pNode;
			int depth;
			int start;
			int end;
			bool bFirstVisit = true;
			TmpNode(Node* iNode, int iDepth, int iStart, int iEnd)
				: pNode(iNode), depth(iDepth), start(iStart), end(iEnd)
			{}
		};

		stack<TmpNode> tStack;
		tStack.push(TmpNode(m_root, 0, 0, m_tmpElements.size()));

		while (!tStack.empty())
		{
			auto tNode = tStack.top();
			tStack.pop();

			auto* pNode = tNode.pNode;

			if (tNode.bFirstVisit)
			{
				// First time visiting this node
				tNode.bFirstVisit = false;

				int& depth = tNode.depth;
				int& start = tNode.start;
				int& end = tNode.end;

				m_maxDepth = (depth > m_maxDepth) ? depth : m_maxDepth;
				int numElements = end - start;
				if (numElements <= MAX_NUM_NODE_ELEMENTS)
				{
					// Leaf node
					for (int i = start; i < end; ++i)
					{
						pNode->elements.push_back(m_tmpElements[i]);
					}
					pNode->box = GetBoundingBox(pNode->elements);
					continue;
				}

				pNode->box.Init();
				int axis = depth % 3;
				Sort(start, end, axis, pNode);

				int median = (start + end) / 2;

				// Push current node back for later bounding box accumulation
				tStack.push(tNode);

				if (end > median)
				{
					pNode->right = new Node();
					tStack.push(TmpNode(pNode->right, depth + 1, median, end));
				}

				if (median > start)
				{
					pNode->left = new Node();
					tStack.push(TmpNode(pNode->left, depth + 1, start, median));
				}
			}
			else
			{
				// Second time visiting this node, accumulate bounding box from children
				if (pNode->left != nullptr)
				{
					pNode->box.Add(pNode->left->box);
				}

				if (pNode->right != nullptr)
				{
					pNode->box.Add(pNode->right->box);
				}
			}
		}

		return m_root;
	}
} // namesapce PotatoEngine
