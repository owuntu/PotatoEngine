#include <iostream>
#include <stack>

#include "BVH.h"

namespace PotatoEngine
{
	BVH::~BVH()
	{
		m_elements.clear();
		m_nodes.clear();
	}

	void BVH::Build(uint32_t numElements)
	{
		std::cout << "Building BVH ...\n";
		BBox box;
		m_elements.resize(numElements);

		for (int i = 0; i < numElements; ++i)
		{
			BBox ebox;
			GetElementBound(i, ebox);
			box += ebox;

			m_elements[i] = i;
		}

		auto* pRoot = new TempNode();
		pRoot->box = box;
		pRoot->elementOffset = 0;
		pRoot->numElements = numElements;

		auto nodesCount = SplitTempNode(pRoot);
		m_nodes.resize(nodesCount + GetRootNodeID());
		ConvertTreeNodesIntoArray(pRoot, GetRootNodeID(), GetRootNodeID() + 1);
		ClearTempNodes(pRoot);
	}

	uint32_t BVH::SplitTempNode(TempNode* pNode)
	{
		uint32_t nodesCount = 1;
		if (pNode->numElements <= ms_MAX_LEAF_ELEMENT_COUNT)
		{
			// we reach the leaf node
			return nodesCount;
		}

		unsigned int child1NumElements = MeanSplit(pNode);
		if (child1NumElements == 0 || child1NumElements >= pNode->numElements)
		{
			// Force split
			child1NumElements = pNode->numElements / 2;
		}

		TempNode* child1 = new TempNode();

		child1->elementOffset = pNode->elementOffset;
		child1->numElements = child1NumElements;

		for (unsigned int i = 0; i < child1NumElements; ++i)	
		{
			unsigned int index = m_elements[i + (child1->elementOffset)];
			BBox tbox;
			GetElementBound(index, tbox);
			child1->box += tbox;
		}

		TempNode* child2 = new TempNode();

		child2->elementOffset = pNode->elementOffset + child1NumElements;
		child2->numElements = pNode->numElements - child1NumElements;
		for (unsigned int i = 0; i < child2->numElements; ++i)
		{
			unsigned int index = m_elements[i + (child2->elementOffset)];
			BBox tbox;
			GetElementBound(index, tbox);
			child2->box += tbox;
		}

		nodesCount += SplitTempNode(child1);
		nodesCount += SplitTempNode(child2);

		pNode->child1 = child1;
		pNode->child2 = child2;

		return nodesCount;
	}


	unsigned int BVH::MeanSplit(TempNode* node)
	{
		const auto& box = node->box;
		glm::vec3 d = box.vmax - box.vmin;

		// Find the maximum dimension of the box and try to split from it
		unsigned int splitAxis[3];
		splitAxis[0] = (d[0] > d[1] && d[0] > d[2]) ? 0 : (d[1] > d[2] ? 1 : 2);
		splitAxis[1] = (splitAxis[0] + 1) % 3;
		splitAxis[2] = (splitAxis[0] + 2) % 3;

		if (d[splitAxis[1]] < d[splitAxis[2]])
		{
			auto t = splitAxis[1];
			splitAxis[1] = splitAxis[2];
			splitAxis[2] = t;
		}

		unsigned int child1NumElements = 0;

		// Iterate throgh each dimension in case we can't find a valid split
		for (unsigned int dim = 0; dim < 3; ++dim)
		{
			auto axis = splitAxis[dim];
			float splitPos = (box.vmax[axis] + box.vmin[axis]) * 0.5f;

			auto i = node->elementOffset;
			auto j = node->numElements + node->elementOffset;
			while (i < j)
			{
				float center = GetElementCenter(m_elements[i], axis);
				if (center <= splitPos)
				{
					i++;
				}
				else
				{
					// swap it to the right side
					j--;
					auto t = m_elements[i];
					m_elements[i] = m_elements[j];
					m_elements[j] = t;
				}
			}

			if (i > node->elementOffset && i < node->numElements + node->elementOffset)
			{
				child1NumElements = i - node->elementOffset;
				break;
			}
		}

		return child1NumElements;
	}

	std::size_t BVH::ConvertTreeNodesIntoArray(TempNode* pNode, uint32_t nodeID, uint32_t child1Index)
	{
		if (pNode->child1 == nullptr)
		{
			m_nodes[nodeID].SetLeafNode(pNode->box, pNode->numElements, pNode->elementOffset);
			// Return the unused child1Index for other internal nodes
			return child1Index;
		}

		m_nodes[nodeID].SetInternalNode(pNode->box, child1Index);
		auto newChildIndex = ConvertTreeNodesIntoArray(pNode->child1, child1Index, child1Index + 2);
		// Use the unused newChildIndex for child2->child1 nodeID
		return ConvertTreeNodesIntoArray(pNode->child2, child1Index + 1, newChildIndex);
	}

	void BVH::ClearTempNodes(TempNode* pRoot)
	{
		if (pRoot != nullptr)
		{
			std::stack<TempNode*> nodeStack;
			nodeStack.push(pRoot);

			while (!nodeStack.empty())
			{
				auto* pNode = nodeStack.top();
				nodeStack.pop();

				if (pNode->child1 != nullptr)
					nodeStack.push(pNode->child1);
				if (pNode->child2 != nullptr)
					nodeStack.push(pNode->child2);
				delete pNode;
			}
		}
	}

} // namespace PotatoEngine
