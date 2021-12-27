#ifndef POTATO_ENGINE_MODEL_H_
#define POTATO_ENGINE_MODEL_H_

#include <iostream>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Model.h"
#include "Mesh.h"

struct aiNode;
struct aiScene;
struct aiMesh;


namespace PotatoEngine
{
	class MeshModel : public Model
	{
	public:
		MeshModel()
		{}
		
		~MeshModel() {}

		void Draw() const;
		void DrawVertices() const;

	private:
		std::vector<Mesh> m_meshes;

		void ProcessMesh(aiMesh* pMesh);
	}; // class MeshModel
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MODEL_H_
