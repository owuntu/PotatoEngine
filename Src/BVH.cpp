#include <cassert>
#include <iostream>
#include <stack>

#include "HelperDraw.h"
#include "BVH.h"

namespace PotatoEngine
{
	void BVH::ArrayNode::SetLeafNode(const BBox& iBox, uint32_t elementCount, uint32_t elementOffset)
	{
		assert(elementCount != 0);
		assert(elementCount <= ms_MAX_LEAF_ELEMENT_COUNT);
		m_box = iBox;
		m_data = ((static_cast<uint32_t>(elementCount - 1) << ms_ELEMENT_OFFSET_BITS) | (elementOffset & ms_ELEMENT_OFFSET_MASK) | ms_LEAF_NODE_MASK);
	}

	void BVH::ArrayNode::SetInternalNode(const BBox& iBox, uint32_t child1Index)
	{
		assert(child1Index <= ms_CHILD_INDEX_MASK);
		m_box = iBox;
		m_data = (child1Index & ms_CHILD_INDEX_MASK);
	}

	bool BVH::ArrayNode::IsLeafNode() const
	{
		return (m_data & ms_LEAF_NODE_MASK) != 0;
	}

	// Must be leaf node
	uint32_t BVH::ArrayNode::GetElementCount() const
	{
		assert(IsLeafNode());
		return ((m_data >> ms_ELEMENT_OFFSET_BITS) & ms_ELEMENT_COUNT_MASK );
	}

	// Must be leaf node
	uint32_t BVH::ArrayNode::GetElementOffset() const
	{
		assert(IsLeafNode());
		return (m_data & ms_ELEMENT_OFFSET_MASK);
	}

	// Must be internal node
	uint32_t BVH::ArrayNode::GetChild1Index() const
	{
		assert(!IsLeafNode());
		return (m_data & ms_CHILD_INDEX_MASK);
	}

	// Must be internal node
	uint32_t BVH::ArrayNode::GetChild2Index() const
	{
		assert(!IsLeafNode());
		return ((m_data & ms_CHILD_INDEX_MASK) + 1);
	}

	BVH::~BVH()
	{
		if (m_root != nullptr)
		{
			std::stack<Node*> nodeStack;
			nodeStack.push(m_root);

			while (!nodeStack.empty())
			{
				auto* node = nodeStack.top();
				nodeStack.pop();

				if (node->child1 != nullptr)
					nodeStack.push(node->child1);
				if (node->child2 != nullptr)
					nodeStack.push(node->child2);
				delete node;
			}
		}

		m_root = nullptr;
		
	}

	void BVH::Build(int numElements)
	{
		BBox box;
		box.Init();
		m_elements.resize(numElements);

		for (int i = 0; i < numElements; ++i)
		{
			BBox ebox;
			ebox.Init();
			GetElementBound(i, ebox);
			box += ebox;

			m_elements[i] = i;
		}

		m_root = new Node();
		m_root->box = box;
		m_root->elementOffset = 0;
		m_root->numElements = numElements;

		auto nodesCount = SplitNode(m_root);
		m_nodes.resize(nodesCount + GetRootNodeID());
		ConvertTreeNodesIntoArray(m_root, GetRootNodeID(), GetRootNodeID() + 1);
	}

	uint32_t BVH::SplitNode(Node* node)
	{
		uint32_t nodesCount = 1;
		if (node->numElements <= ms_MAX_LEAF_ELEMENT_COUNT)
		{
			// we reach the leaf node
			return nodesCount;
		}

		unsigned int child1NumElements = MeanSplit(node);
		if (child1NumElements == 0 || child1NumElements >= node->numElements)
		{
			// Force split
			child1NumElements = node->numElements / 2;
		}

		Node* child1 = new Node();

		child1->elementOffset = node->elementOffset;
		child1->numElements = child1NumElements;

		for (unsigned int i = 0; i < child1NumElements; ++i)	
		{
			unsigned int index = m_elements[i + (child1->elementOffset)];
			BBox tbox;
			GetElementBound(index, tbox);
			child1->box += tbox;
		}

		Node* child2 = new Node();

		child2->elementOffset = node->elementOffset + child1NumElements;
		child2->numElements = node->numElements - child1NumElements;
		for (unsigned int i = 0; i < child2->numElements; ++i)
		{
			unsigned int index = m_elements[i + (child2->elementOffset)];
			BBox tbox;
			GetElementBound(index, tbox);
			child2->box += tbox;
		}

		nodesCount += SplitNode(child1);
		nodesCount += SplitNode(child2);

		node->child1 = child1;
		node->child2 = child2;

		return nodesCount;
	}


	unsigned int BVH::MeanSplit(Node* node)
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
			unsigned int axis = splitAxis[dim];
			float splitPos = (box.vmax[axis] + box.vmin[axis]) * 0.5f;

			int i = node->elementOffset;
			int j = node->numElements + node->elementOffset;
			while (i < j)
			{
				glm::vec3 center = GetElementCenter(m_elements[i]);
				if (center[axis] <= splitPos)
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

	std::size_t BVH::ConvertTreeNodesIntoArray(Node* pNode, std::size_t nodeID, std::size_t child1Index)
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

	void BVH::DebugDrawBVH(PotatoEngine::ShaderProgram* pShader, int depthToDraw)
	{
		DebugDraw(pShader, 0, 0, depthToDraw);
	}

	void BVH::DebugDraw(ShaderProgram* pShader, std::size_t nodeID, int depth, int depthToDraw)
	{
		static glm::vec4 colors[3] = {
			{0.8f, 0, 0, 1.f},
			{0, 0.8f, 0, 1.f},
			{0, 0.8f, 0.8f, 1.f}
		};

		if (depth > depthToDraw)
		{
			return;
		}

		const auto& node = m_nodes[nodeID];
		if (depth == depthToDraw)
		{
			pShader->SetMat4("modelMat", glm::mat4(1.0f));
			pShader->SetVec4("ModelColor", colors[depth % 3]);
			DrawBox(node.GetBoundingBox());
		}

		if (node.IsLeafNode())
		{
			return;
		}

		DebugDraw(pShader, node.GetChild1Index(), depth + 1, depthToDraw);
		DebugDraw(pShader, node.GetChild2Index(), depth + 1, depthToDraw);
	}

} // namespace PotatoEngine
