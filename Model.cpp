#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Model.h"

namespace PotatoEngine
{
	void Model::LoadModel()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_directory, aiProcess_Triangulate);

		// check for errors
		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
			return;
		}

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
