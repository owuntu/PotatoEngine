#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "MeshModel.h"


namespace PotatoEngine
{
	void MeshModel::ProcessMesh(aiMesh* pMesh)
	{
		std::vector<Vertex> vertices(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
		{
			Vertex v;
			v.Position.x = pMesh->mVertices[i].x;
			v.Position.y = pMesh->mVertices[i].y;
			v.Position.z = pMesh->mVertices[i].z;
			vertices[i] = v;
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

		m_tmpMeshes.push_back(Mesh(vertices, indices));
	}

	void MeshModel::PostSetup()
	{
		// Merging all meshes into one.
		// todo: provide option to whether to merge
		using namespace std;

		std::size_t numVertices = 0;
		std::size_t numIndices = 0;

		for (std::size_t i = 0; i < m_tmpMeshes.size(); ++i)
		{
			auto& mesh = m_tmpMeshes[i];
			numVertices += mesh.m_vertices.size();
			numIndices += mesh.m_indices.size();
		}

		vector<Vertex> vertices(numVertices);
		vector<unsigned int> indices(numIndices);

		std::size_t vertexOffset = 0;
		std::size_t indexOffset = 0;
		for (std::size_t i = 0; i < m_tmpMeshes.size(); ++i)
		{
			auto& mesh = m_tmpMeshes[i];
			memcpy(&vertices[vertexOffset], &mesh.m_vertices[0], mesh.m_vertices.size() * sizeof(Vertex));

			for (std::size_t ii = 0; ii < mesh.m_indices.size(); ii += 3)
			{
				indices[indexOffset + ii] = mesh.m_indices[ii] + vertexOffset;
				indices[indexOffset + ii + 1] = mesh.m_indices[ii + 1] + vertexOffset;
				indices[indexOffset + ii + 2] = mesh.m_indices[ii + 2] + vertexOffset;
			}
			vertexOffset += mesh.m_vertices.size();
			indexOffset += mesh.m_indices.size();
		}

		m_tmpMeshes.clear();
		m_mesh = Mesh(vertices, indices);
	}

} // PotatoEngine
