#ifndef POTATO_ENGINE_MODEL_H_
#define POTATO_ENGINE_MODEL_H_

#include <iostream>
#include <memory>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.h"

namespace PotatoEngine
{
	class Model
	{
	public:
		std::string m_directory;

		Model(const std::string& path)
		{
			this->LoadModel(path);
		}
		
		~Model() {}

	private:
		std::vector<Mesh> m_meshes;

		void LoadModel(const std::string& path);
		void ProcessNode(std::shared_ptr<aiNode> pNode, std::shared_ptr<aiScene> pScene);
		void ProcessMesh(std::shared_ptr<aiMesh> pMesh);
	}; // class Model
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MODEL_H_
