#ifndef POTATOENGINE_BBOX_H_
#define POTATOENGINE_BBOX_H_

#include <glm\glm.hpp>

namespace PotatoEngine
{
	struct BBox
	{
		glm::vec3 vmin;
		glm::vec3 vmax;

		BBox()
		{
			Init();
		}

		void Init()
		{
			vmin = glm::vec3(FLT_MAX);
			vmax = glm::vec3(-FLT_MAX);
		}

		void Add(const glm::vec3& p)
		{
			for (int i = 0; i < 3; ++i)
			{
				vmin[i] = fminf(p[i], vmin[i]);
				vmax[i] = fmaxf(p[i], vmax[i]);
			}
		}

		void Add(const BBox& rhs)
		{
			for (int i = 0; i < 3; ++i)
			{
				vmin[i] = fminf(rhs.vmin[i], vmin[i]);
				vmax[i] = fmaxf(rhs.vmax[i], vmax[i]);
			}
		}
	};
}


#endif // POTATOENGINE_BBOX_H_