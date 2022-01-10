#include <queue>

#include "KdTreeFixed.h"

namespace PotatoEngine
{
	void KdTreeFixed::Build()
	{
		KdTree::Build();

		// Find how many nodes
		int depth = KdTree::GetMaxDepth();
		int numNodes = (1 << (depth + 1)) - 1;

		if (m_nodeArry != nullptr)
		{
			delete[] m_nodeArry;
		}

		m_nodeArry = new NodeFix[numNodes];

		struct TmpNode
		{
			Node* pNode = nullptr;
			int offset = 0;
			TmpNode(Node* node, int o) : pNode(node), offset(o) {}
			TmpNode(const TmpNode& other) : pNode(other.pNode), offset(other.offset) {}
		};

		std::queue<TmpNode> queue;
		queue.push(TmpNode(m_root, 0));

		while (queue.empty() == false)
		{
			auto node = queue.front();
			queue.pop();

			NodeFix nodeFix;

			Node* pNode = node.pNode;

			nodeFix.box = pNode->box;

			if (!pNode->elements.empty())
			{
				// is leaf
				nodeFix.numElements = pNode->elements.size();
				nodeFix.bIsLeaf = true;
				for (int i = 0; i < nodeFix.numElements; ++i)
				{
					nodeFix.elements[i] = pNode->elements[i];
				}
			}
			else
			{
				nodeFix.nodeData.splitAxis = pNode->splitAxis;
				nodeFix.nodeData.splitPos = pNode->splitPos;
			}

			m_nodeArry[node.offset] = nodeFix;

			int off2 = node.offset << 1;
			// continue on children
			if (pNode->left != nullptr)
			{
				queue.push(TmpNode(pNode->left, off2 + 1));
			}
			if (pNode->right != nullptr)
			{
				queue.push(TmpNode(pNode->right, off2 + 2));
			}
		}
	}
} // namespacce PotatoEngine
