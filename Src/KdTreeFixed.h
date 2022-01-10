#ifndef POTATOENGINE_KD_TREE_FIXED_H_
#define POTATOENGINE_KD_TREE_FIXED_H_

#include "KdTree.h"

namespace PotatoEngine
{
	class KdTreeFixed : public KdTree
	{
	protected:
		struct NodeFix
		{
			bool bIsLeaf = false;
			int numElements = 0;
			BBox box;
			struct InternalNode
			{
				int splitAxis = -1;
				float splitPos = 0.f;
			};
			union
			{
				InternalNode nodeData;
				int elements[MAX_NUM_NODE_ELEMENTS] = { 0 };
			};

			NodeFix() {}
		};

		NodeFix* m_nodeArry = nullptr;

		virtual void Build();
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_KD_TREE_FIXED_H_
