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
		unsigned int child1EleCount = MeanSplit(node);
		if (child1EleCount == 0)
		{
			if (node->numElements > ms_maxLeafElements)
			{
				// we must split
				// todo:
				child1EleCount = node->numElements / 2;
			}
			else
			{
				// we reach the leaf node
				return;
			}
		}

		BBox box;

		Node* child1 = new Node();
		box.Init();

		child1->elementOffset = node->elementOffset;
		child1->numElements = child1EleCount;

		for (unsigned int i = child1->elementOffset; i < child1EleCount; ++i)	
		{
			auto index = m_elements[i];
			BBox tbox;
			GetElementBound(index, tbox);
			box += tbox;
		}

		Node* child2 = new Node();
		box.Init();

		child2->elementOffset = child1EleCount;
		child2->numElements = node->numElements - child1EleCount;
		for (unsigned int i = child2->elementOffset; i < child2->numElements; ++i)
		{
			auto index = m_elements[i];
			BBox tbox;
			GetElementBound(index, tbox);
			box += tbox;
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
		unsigned int splitAxis = (d[0] > d[1] && d[0] > d[2]) ? 0 : (d[1] > d[2] ? 1 : 2);

		int i = node->elementOffset;
		int j = node->numElements;

		unsigned int child1EleCount = 0;

		// Iterate throgh each dimension in case we can't find a valid split
		for (unsigned int dim = 0; dim < 3; ++dim)
		{
			splitAxis = (splitAxis + 1) % 3;
			float splitPos = (box.vmax + box.vmin)[splitAxis] * 0.5f;

			while (i < j)
			{
				glm::vec3 center = GetElementCenter(i);
				if (center[splitAxis] < splitPos)
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

			if (i > 0 && i < node->numElements)
			{
				child1EleCount = i;
				break;
			}

		}

		return child1EleCount;
	}

} // namespace PotatoEngine
