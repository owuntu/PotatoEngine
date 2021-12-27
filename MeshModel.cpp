#include "Model.h"

namespace PotatoEngine
{
	void MeshModel::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

		// check for errors
		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
			return;
		}

		m_directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
	}

	void MeshModel::ProcessNode(aiNode* pNode, const aiScene* pScene)
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

	void MeshModel::ProcessMesh(aiMesh* pMesh)
	{
		std::vector<Vertex> vertices;
		for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
		{
			Vertex v;
			v.Position.x = pMesh->mVertices[i].x;
			v.Position.y = pMesh->mVertices[i].y;
			v.Position.z = pMesh->mVertices[i].z;

			if (pMesh->HasNormals())
			{
				v.Normal.x = pMesh->mNormals[i].x;
				v.Normal.y = pMesh->mNormals[i].y;
				v.Normal.z = pMesh->mNormals[i].z;
			}
			vertices.push_back(v);
		}

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
		{
			aiFace face = pMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		m_meshes.push_back(Mesh(vertices, indices));
	}

	void MeshModel::Draw() const
	{
		for (const auto& mesh : m_meshes)
		{
			mesh.Draw();
		}
	}

	void MeshModel::DrawVertices() const
	{
		for (const auto& mesh : m_meshes)
		{
			mesh.DrawVertices();
		}
	}

} // PotatoEngine
