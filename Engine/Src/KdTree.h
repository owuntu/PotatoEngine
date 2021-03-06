#ifndef POTATOeNGINE_KD_TREE_H_
#define POTATOeNGINE_KD_TREE_H_

#include <glm/glm.hpp>
#include <vector>

#include "BBox.h"

namespace PotatoEngine
{
	class KdTree
	{
	public:
		static const int MAX_NUM_NODE_ELEMENTS;
		
		struct Node
		{
			Node* left = nullptr;
			Node* right = nullptr;
			int splitAxis = -1;
			float splitPos = 0.f;

			// Bounding box
			BBox box;

			// element indices to original array
			// This vector is non-empty only if the node is a leaf node 
			std::vector<int> elements;
		}; // struct Node

		KdTree() {}

		void Build();

		const Node* GetRoot() const { return m_root; }
		const int GetMaxDepth() const { return m_maxDepth; }
	protected:
		virtual std::size_t GetTotalNumOfElements() const = 0;
		virtual void Sort(int start, int end, int axis, Node* node) = 0;
		virtual BBox GetBoundingBox(const std::vector<int>& elements) const = 0;

		virtual int GetSplitAxis(int start, int end, int depth) = 0;

		Node* m_root = nullptr;
		std::vector<int> m_tmpElements; // All element indices to original array
	private:
		Node* BuildTree(int start, int end, int depth);

		// todo: Need to be fixed. There is overlap split plane using iteration.
		//Node* BuildTreeIteration();

		// Disabled copy constructor for now
		KdTree(const KdTree& rhs) = delete;

		int m_maxDepth = 0;
	};
} // namespace PotatoEngine

#endif //POTATOeNGINE_KD_TREE_H_
