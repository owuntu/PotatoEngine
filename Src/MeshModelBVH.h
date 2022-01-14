#ifndef POTATOENGINE_MESH_MODEL_BVH_H_
#define POTATOENGINE_MESH_MODEL_BVH_H_

#include "MeshModel.h"
#include "BVH.h"

namespace PotatoEngine
{
	class MeshModelBVH : public MeshModel, public BVH
	{
	public:
		virtual ~MeshModelBVH() {}
	protected:

		virtual void GetElementBound(int index, BBox& box);
		virtual glm::vec3 GetElementCenter(int index);

	private:
		virtual void PostSetup();

	}; // class MeshModelBVH

} // namespace PotatoEngine


#endif // POTATOENGINE_MESH_MODEL_BVH_H_
