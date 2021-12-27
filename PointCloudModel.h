#ifndef POTATO_ENGINE_POINT_CLOUD_MODEL_H_
#define POTATO_ENGINE_POINT_CLOUD_MODEL_H_

#include <vector>

#include <glm/glm.hpp>

#include "Model.h"

namespace PotatoEngine
{
	class PointCloudModel : public Model
	{
	public:
		PointCloudModel(const std::string& path) : Model(path),
			m_glVAO(0),
			m_glVBO(0)
		{}

		~PointCloudModel() {}

		void Draw() const;
		void DrawVertices() const;

		// todo: refactor GL object into another class
		unsigned int m_glVAO;

	private:
		// All mesh vertex point are just put together
		std::vector<glm::vec3> m_points;

		void ProcessMesh(aiMesh* pMesh);

		void PostSetup();

		// todo: refactor GL object into another class
		unsigned int m_glVBO;
	};
} // namespace PotatoEngine

#endif // POTATO_ENGINE_POINT_CLOUD_MODEL_H_
