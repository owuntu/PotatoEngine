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
		struct Node
		{
			Node* left = nullptr;
			Node* right = nullptr;
			int splitAxis = -1;
			float splitPos = 0.f;

			BBox box;

			std::vector<int> elements; // element indices to original array
		}; // class Node

		KdTree() {}

		void Build();

		Node* BuildTree(std::vector<int> elements, int depth);

		const Node* GetRoot() const { return m_root; }

	protected:
		virtual int GetTotalNumOfElements() const = 0;
		virtual void Sort(std::vector<int>& elements, int axis, Node* node) = 0;
		virtual BBox GetBoundingBox(const std::vector<int>& elements) const = 0;

		Node* m_root = nullptr;

	private:
		// Disabled copy constructor for now
		KdTree(const KdTree& rhs) = delete;
	};
} // namespace PotatoEngine

#endif //POTATOeNGINE_KD_TREE_H_
