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
		
		virtual ~MeshModel()
		{
			m_mesh.Release();
		}

		const Mesh& GetMesh() const { return m_mesh; }

	protected:
		virtual void PostSetup();

	private:
		void ProcessMesh(aiMesh* pMesh);

		void DoDraw() const;
		void DoDrawVertices() const;

		std::vector<Mesh> m_tmpMeshes;

		// merge into single mesh
		Mesh m_mesh;
	}; // class MeshModel
} // namespace PotatoEngine

#endif // POTATO_ENGINE_MODEL_H_
