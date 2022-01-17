#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Model.h"

namespace PotatoEngine
{
	void Model::LoadModel(const std::string& objModelPath)
	{
		Assimp::Importer importer;
		// todo: provide option for whether to join identical vertices.
		const aiScene* scene = importer.ReadFile(objModelPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		// check for errors
		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			std::string msg = "Error loading model: ";
			msg += importer.GetErrorString();
			std::cerr << msg << std::endl;
			throw std::runtime_error(msg);
		}

		m_directory = objModelPath;

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* pNode, const aiScene* pScene)
	{
		for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
		{
			auto* pMesh = pScene->mMeshes[pNode->mMeshes[i]];

			ProcessMesh(pMesh);
		}

		for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
		{
			ProcessNode(pNode->mChildren[i], pScene);
		}
	}

} // namespace PotatoEngine
