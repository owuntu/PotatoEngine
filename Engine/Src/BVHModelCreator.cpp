#include "MeshModelBVH.h"
#include "BVHModelCreator.h"

namespace PotatoEngine
{
	std::shared_ptr<Model> BVHModelCreator::DoCreateModel(const Type t)
	{
		if (t != Type::MESH_MODEL)
		{
			// only supporting MeshModel for now
			return ModelCreator::DoCreateModel(t);
		}

		return std::make_shared<MeshModelBVH>();
	}
}
