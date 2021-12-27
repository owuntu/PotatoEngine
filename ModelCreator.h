#ifndef POTATO_ENGINE_MODEL_CREATOR_H_
#define POTATO_ENGINE_MODEL_CREATOR_H_

#include <memory>
#include <string>

#include "Model.h"
#include "MeshModel.h"
#include "PointCloudModel.h"

namespace PotatoEngine
{
	namespace ModelCreator
	{
		enum class Type
		{
			MESH_MODEL,
			POINT_CLOUD_MODEL,
			COUNT
		};
		std::shared_ptr<Model> CreateModel(const Type t, const std::string& path);
	}; // namespace ModelCreator
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MODEL_CREATOR_H_
