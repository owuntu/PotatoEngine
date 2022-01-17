#include "ModelCreator.h"

namespace PotatoEngine
{
	std::shared_ptr<Model> ModelCreator::CreateModel(const Type t, const std::string& objModelPath)
	{
		std::shared_ptr<Model> pModel = DoCreateModel(t);
		if (pModel != nullptr && objModelPath != "")
		{
			pModel->LoadModel(objModelPath);
		}

		pModel->PostSetup();

		return pModel;
	}

	std::shared_ptr<Model> ModelCreator::DoCreateModel(const Type t)
	{
		std::shared_ptr<Model> pModel = nullptr;
		switch (t)
		{
		case Type::MESH_MODEL:
			pModel = std::make_shared<MeshModel>();
			break;
		case Type::POINT_CLOUD_MODEL:
			pModel = std::make_shared<PointCloudModel>();
			break;
		case Type::SINGLE_POINT_MODEL:
			pModel = std::make_shared<SinglePointModel>();
			break;
		default:
			break;
		}

		return pModel;
	}

} // namespace PotatoEngine
