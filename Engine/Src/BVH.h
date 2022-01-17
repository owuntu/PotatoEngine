#ifndef POTATOENGINE_BVH_H_
#define POTATOENGINE_BVH_H_
#include <cassert>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

#include "BBox.h"

namespace PotatoEngine
{
	class ShaderProgram;

	class BVH
	{
	public:
		static const uint32_t ms_ELEMENT_COUNT_BITS = 3;
		static const uint32_t ms_MAX_LEAF_ELEMENT_COUNT = (1 << ms_ELEMENT_COUNT_BITS);
		// Element count should not be 0
		static const uint32_t ms_ELEMENT_COUNT_MASK = (1 << ms_ELEMENT_COUNT_BITS) - 1;

		static const uint32_t ms_NODE_DATA_BITS = sizeof(uint32_t) * 8;
		static const uint32_t ms_LEAF_NODE_BIT = 1;
		static const uint32_t ms_LEAF_NODE_MASK = (static_cast<uint32_t>(1) << (ms_NODE_DATA_BITS - 1));

		static const uint32_t ms_ELEMENT_OFFSET_BITS = ms_NODE_DATA_BITS - ms_ELEMENT_COUNT_BITS - ms_LEAF_NODE_BIT;
		static const uint32_t ms_ELEMENT_OFFSET_MASK = (static_cast<uint32_t>(1) << ms_ELEMENT_OFFSET_BITS) - 1;

		static const uint32_t ms_CHILD_INDEX_BITS = ms_NODE_DATA_BITS - ms_LEAF_NODE_BIT;
		static const uint32_t ms_CHILD_INDEX_MASK = (static_cast<uint32_t>(1) << ms_CHILD_INDEX_BITS) - 1;

		virtual ~BVH();
		void DebugDrawBVH(ShaderProgram* pShader, int depthToDraw);

	protected:
		class Node
		{
		public:
			inline void SetLeafNode(const BBox& iBox, uint32_t elementCount, uint32_t elementOffset)
			{
				assert(elementCount != 0);
				assert(elementCount <= ms_MAX_LEAF_ELEMENT_COUNT);
				m_box = iBox;
				m_data = ((static_cast<uint32_t>(elementCount - 1) << ms_ELEMENT_OFFSET_BITS) | (elementOffset & ms_ELEMENT_OFFSET_MASK) | ms_LEAF_NODE_MASK);
			}

			inline void SetInternalNode(const BBox& iBox, uint32_t child1Index)
			{
				assert(child1Index <= ms_CHILD_INDEX_MASK);
				m_box = iBox;
				m_data = (child1Index & ms_CHILD_INDEX_MASK);
			}

			inline bool IsLeafNode() const { return (m_data & ms_LEAF_NODE_MASK) != 0; }

			// Must be leaf node
			inline uint32_t GetElementCount() const
			{
				assert(IsLeafNode());
				return ((m_data >> ms_ELEMENT_OFFSET_BITS) & ms_ELEMENT_COUNT_MASK) + 1;
			}

			// Must be leaf node
			inline uint32_t GetElementOffset() const
			{
				assert(IsLeafNode());
				return (m_data & ms_ELEMENT_OFFSET_MASK);
			}

			// Must be internal node
			inline uint32_t GetChild1Index() const
			{
				assert(!IsLeafNode());
				return (m_data & ms_CHILD_INDEX_MASK);
			}

			// Must be internal node
			inline uint32_t GetChild2Index() const
			{
				assert(!IsLeafNode());
				return (GetChild1Index() + 1);
			}

			inline const BBox& GetBoundingBox() const { return m_box; }

		private:
			BBox m_box; // 6 float, 6*4 = 24 bytes

			// 1) For leaf node
			// 31,            30, 29, 28,     27, 26, ... 2, 1, 0
			// leaf bit = 1 | element count | element offset
			// 
			// 2) For internal node
			// 31,            30, 29, ... 2, 1, 0
			// leaf bit = 0 | child index
			uint32_t m_data; // 4 bytes
		};

		void Build(uint32_t numElements);
		const Node& GetRoot() const { return m_nodes[GetRootNodeID()]; }

		virtual void GetElementBound(int index, BBox& box) = 0;
		virtual float GetElementCenter(int index, int dim) = 0;

		// Element indices
		std::vector<unsigned int> m_elements;

		// Nodes array for storing the BVH tree
		std::vector<Node> m_nodes;

		inline uint32_t GetRootNodeID() const { return 0; }

	private:
		struct TempNode
		{
			BBox box; // 6 float, 6*4 = 24 bytes

			TempNode* child1 = nullptr; // 64 bit system, 8 bytes
			TempNode* child2 = nullptr; // 8 bytes

			unsigned int numElements = 0; // 4 bytes
			unsigned int elementOffset = 0; // 4 bytes
		};

		// Return total nodes count start from node
		uint32_t SplitTempNode(TempNode* pNode);

		unsigned int MeanSplit(TempNode* pNode);

		// return a unused child1Index for other internal nodes if pNode is a leaf node
		std::size_t ConvertTreeNodesIntoArray(TempNode* pNode, uint32_t nodeID, uint32_t child1Index);

		void ClearTempNodes(TempNode* pRoot);
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_BVH_H_
