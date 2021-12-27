#include "ModelCreator.h"

namespace PotatoEngine
{
	std::shared_ptr<Model> ModelCreator::CreateModel(const Type t, const std::string& path)
	{
		std::shared_ptr<Model> pModel = nullptr;
		switch (t)
		{
		case Type::MESH_MODEL:
			pModel = std::make_shared<MeshModel>(path);
			break;
		default:
			break;
		}

		if (pModel != nullptr)
		{
			pModel->LoadModel();
		}

		return pModel;
	}
} // namespace PotatoEngine
