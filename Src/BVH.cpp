#include <stack>
#include "BVH.h"

namespace PotatoEngine
{
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

		SplitNode(m_root);
	}

	void BVH::SplitNode(Node* node)
	{
		if (node->numElements <= ms_maxLeafElements)
		{
			// we reach the leaf node
			return;
		}

		unsigned int child1NumElements = MeanSplit(node);
		if (child1NumElements == 0 || child1NumElements >= node->numElements)
		{
			// we must split
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

		SplitNode(child1);
		SplitNode(child2);

		node->child1 = child1;
		node->child2 = child2;
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
				child1NumElements = i;
				break;
			}
		}

		return child1NumElements;
	}

} // namespace PotatoEngine
