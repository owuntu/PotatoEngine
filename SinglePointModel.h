#ifndef SINGLE_POINT_MODEL_H_
#define SINGLE_POINT_MODEL_H_

#include <glm/glm.hpp>

#include "Model.h"

struct aiMesh;

namespace PotatoEngine
{
	class SinglePointModel : public Model
	{
	public:
		SinglePointModel() :
			m_glVAO(0),
			m_glVBO(0),
			m_point(0)
		{}

		~SinglePointModel() {}

		void Draw() const;
		void DrawVertices() const;

		const glm::vec3& GetPoint() const { return m_point; }
		glm::vec3& GetPoint() { return m_point; }

	private:
		// All mesh vertex point are just put together
		glm::vec3 m_point;

		void ProcessMesh(aiMesh* pMesh) {}

		void PostSetup();

		// todo: refactor GL object into another class
		unsigned int m_glVAO;
		unsigned int m_glVBO;
	};
} // namespace PotatoEngine

#endif // SINGLE_POINT_MODEL_H_
