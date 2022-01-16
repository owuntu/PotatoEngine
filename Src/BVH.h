#ifndef POTATOENGINE_BVH_H_
#define POTATOENGINE_BVH_H_

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

		struct Node
		{
			BBox box; // 6 float, 6*4 = 24 bytes

			Node* child1 = nullptr; // 64 bit system, 8 bytes
			Node* child2 = nullptr; // 8 bytes

			unsigned int numElements = 0; // 4 bytes
			unsigned int elementOffset = 0; // 4 bytes
		};

		class ArrayNode
		{
		public:
			void SetLeafNode(const BBox& iBox, uint32_t elementCount, uint32_t elementOffset);
			void SetInternalNode(const BBox& iBox, uint32_t child1Index);

			bool IsLeafNode() const;

			// Must be leaf node
			uint32_t GetElementCount() const;
			uint32_t GetElementOffset() const;

			// Must be internal node
			uint32_t GetChild1Index() const;
			uint32_t GetChild2Index() const;

			const BBox& GetBoundingBox() const { return m_box; }

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

		const ArrayNode& GetRoot() const { return m_nodes[GetRootNodeID()]; }

		void DebugDrawBVH(ShaderProgram* pShader, int depthToDraw);

	protected:
		virtual ~BVH();
		void Build(int numElements);

		virtual void GetElementBound(int index, BBox& box) = 0;
		virtual float GetElementCenter(int index, int dim) = 0;

		// Element indices
		std::vector<unsigned int> m_elements;

		// Nodes array for storing the BVH tree
		std::vector<ArrayNode> m_nodes;

		std::size_t GetRootNodeID() const { return 0; }

	private:
		uint32_t SplitNode(Node* node);
		unsigned int MeanSplit(Node* node);

		// return a unused child1Index for other internal nodes if pNode is a leaf node
		std::size_t ConvertTreeNodesIntoArray(Node* pNode, std::size_t nodeID, std::size_t child1Index);

		void ClearTempNodes();

		void DebugDraw(ShaderProgram* pShader, std::size_t nodeID, int depth, int depthToDraw);

		Node* m_root = nullptr;
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_BVH_H_
