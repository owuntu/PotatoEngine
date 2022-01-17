// Factory class to create Model
#ifndef POTATO_ENGINE_MODEL_CREATOR_H_
#define POTATO_ENGINE_MODEL_CREATOR_H_

#include <memory>
#include <string>

#include "Model.h"

namespace PotatoEngine
{
	class ModelCreator
	{
	public:
		enum class Type
		{
			MESH_MODEL,
			POINT_CLOUD_MODEL,
			SINGLE_POINT_MODEL,
			COUNT
		};
		std::shared_ptr<Model> CreateModel(const Type t, const std::string& objModelPath = "");
	protected:
		virtual std::shared_ptr<Model> DoCreateModel(const Type t);
	}; // namespace ModelCreator
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MODEL_CREATOR_H_
