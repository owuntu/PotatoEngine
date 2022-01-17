#ifndef SINGLE_POINT_MODEL_H_
#define SINGLE_POINT_MODEL_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"

struct aiMesh;

namespace PotatoEngine
{
	class SinglePointModel : public Model
	{
	public:
		SinglePointModel()
		{}

		~SinglePointModel() {}

		glm::vec3 GetPosition() const
		{
			glm::vec3 p = m_transformation[3];
			return p;
		}

		void SetPosition(const glm::vec3& pos)
		{
			glm::vec4 v(pos, 1.0f);
			m_transformation[3] = v;
		}

		void Translate(const glm::vec3& v)
		{
			glm::translate(m_transformation, v);
		}

	private:
		void ProcessMesh(aiMesh* pMesh) {}
	};
} // namespace PotatoEngine

#endif // SINGLE_POINT_MODEL_H_
