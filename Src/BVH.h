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

	private:
		struct Node
		{
			BBox box;

			Node* child1 = nullptr;
			Node* child2 = nullptr;

			unsigned int numElements = 0;
			unsigned int elementOffset = 0;
		};

	protected:
		virtual ~BVH();
		void Build(int numElements);

		virtual void GetElementBound(int index, BBox& box) = 0;
		virtual glm::vec3 GetElementCenter(int index) = 0;

	private:
		void SplitNode(Node* node);
		unsigned int MeanSplit(Node* node);

		Node* m_root = nullptr;

		// Element indices
		std::vector<unsigned int> m_elements;
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_BVH_H_
