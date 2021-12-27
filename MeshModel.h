#ifndef POTATO_ENGINE_MODEL_H_
#define POTATO_ENGINE_MODEL_H_

#include <iostream>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.h"

namespace PotatoEngine
{
	class MeshModel
	{
	public:
		std::string m_directory;

		MeshModel(const std::string& path)
		{
			this->LoadModel(path);
		}
		
		~MeshModel() {}

		void Draw() const;
		void DrawVertices() const;

	private:
		std::vector<Mesh> m_meshes;

		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* pNode, const aiScene* pScene);
		void ProcessMesh(aiMesh* pMesh);
	}; // class Model
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MODEL_H_
