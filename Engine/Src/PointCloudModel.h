#ifndef POTATO_ENGINE_POINT_CLOUD_MODEL_H_
#define POTATO_ENGINE_POINT_CLOUD_MODEL_H_

#include <vector>

#include <glm/glm.hpp>

#include "Model.h"
#include "KdTree.h"

namespace PotatoEngine
{
	class PointCloudModel : public Model
	{
	public:
		PointCloudModel() :
			m_glVAO(0),
			m_glVBO(0)
		{}

		~PointCloudModel() {}

		const std::vector<glm::vec3>& GetPoints() const { return m_points; }
		std::vector<glm::vec3>& GetPoints() { return m_points; }

	protected:
		virtual void PostSetup();

		// All mesh vertex point are just put together
		std::vector<glm::vec3> m_points;

	private:
		void ProcessMesh(aiMesh* pMesh);
		void DoDraw() const;

		// todo: refactor GL object into another class
		unsigned int m_glVAO;
		unsigned int m_glVBO;
	};
} // namespace PotatoEngine

#endif // POTATO_ENGINE_POINT_CLOUD_MODEL_H_
