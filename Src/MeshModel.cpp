#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "MeshModel.h"


namespace PotatoEngine
{
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

	void MeshModel::PostSetup()
	{
		// Merging all meshes into one.
		// todo: provide option to whether to merge
		using namespace std;
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		std::size_t offset = vertices.size();

		for (std::size_t i = 0; i < m_meshes.size(); ++i)
		{
			auto& mesh = m_meshes[i];
			for (std::size_t vi = 0; vi < mesh.m_vertices.size(); ++vi)
			{
				vertices.push_back(mesh.m_vertices[vi]);
			}

			for (std::size_t ii = 0; ii < mesh.m_indices.size(); ++ii)
			{
				indices.push_back(mesh.m_indices[ii] + offset);
			}
			offset += mesh.m_vertices.size();
			mesh.Release();
		}

		m_meshes.clear();
		m_meshes.push_back(Mesh(vertices, indices));
	}

	void MeshModel::DoDraw() const
	{
		for (const auto& mesh : m_meshes)
		{
			mesh.Draw();
		}
	}

	void MeshModel::DoDrawVertices() const
	{
		for (const auto& mesh : m_meshes)
		{
			mesh.DrawVertices();
		}
	}

} // PotatoEngine
