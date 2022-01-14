#ifndef POTATOENGINE_BVH_H_
#define POTATOENGINE_BVH_H_

#include <vector>
#include <glm/glm.hpp>

#include "BBox.h"

namespace PotatoEngine
{
	class BVH
	{
	public:
		static const unsigned int ms_maxLeafElements = 8;

		struct Node
		{
			BBox box;

			Node* child1 = nullptr;
			Node* child2 = nullptr;

			unsigned int numElements = 0;
			unsigned int elementOffset = 0;
		};

		const Node* GetRoot() const { return m_root; }

	protected:
		virtual ~BVH();
		void Build(int numElements);

		virtual void GetElementBound(int index, BBox& box) = 0;
		virtual glm::vec3 GetElementCenter(int index) = 0;

		Node* m_root = nullptr;

		// Element indices
		std::vector<unsigned int> m_elements;

	private:
		void SplitNode(Node* node);
		unsigned int MeanSplit(Node* node);
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_BVH_H_
