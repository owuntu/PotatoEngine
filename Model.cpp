#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "ShaderObject/ShaderProgram.h"

#include "Model.h"

namespace PotatoEngine
{
	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

		// check for errors
		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
			return;
		}

		m_directory = path;

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

	void Model::Draw(ShaderProgram* pShader) const
	{
		// Pre draw
		if (pShader != nullptr)
		{
			pShader->SetMat4("modelMat", m_transformation);
			pShader->SetVec3("ModelColor", m_color);
		}

		// do draw
		DoDraw();
	}

	void Model::DrawVertices(ShaderProgram* pShader) const
	{
		// Pre draw
		if (pShader != nullptr)
		{
			pShader->SetMat4("modelMat", m_transformation);
			pShader->SetVec3("ModelColor", m_color);
		}

		// do draw
		DoDrawVertices();
	}

} // namespace PotatoEngine
