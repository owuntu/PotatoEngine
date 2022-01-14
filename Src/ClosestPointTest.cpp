#include "ClosestPointTest.h"

namespace PotatoEngine
{
	glm::vec3 ClosestPointOnTriangle1st(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
	{
		// Refering "Real-time Collision Dection" by Erison Christer
		// Section 5.1.5
		// Check the Voronoi region of vertices
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;

		glm::vec3 ap = p - a;
		glm::vec3 bp = p - b;
		glm::vec3 cp = p - c;

		// Project P onto AB as P'
		// P' = A + s * AB, s = s1 / (s1 + s2)
		float s1 = glm::dot(ap, ab), s2 = glm::dot(bp, -ab);

		// Project P onto AC as P'
		// P' = A + t * AC, t = t1 / (t1 + t2)
		float t1 = glm::dot(ap, ac), t2 = glm::dot(cp, -ac);

		// On vertex A region
		if (s1 <= 0.f && t1 <= 0.f)
		{
			return a;
		}

		// Project P onto BC as P'
		// P' = B + r * BC, r = r1 / (r1 + r2)
		glm::vec3 bc = c - b;
		float r1 = glm::dot(bp, bc), r2 = glm::dot(cp, -bc);

		// On vertex B region
		if (s2 <= 0.f && r1 <= 0.f)
		{
			return b;
		}

		// On vertex C region
		if (t2 <= 0.f && r2 <= 0.f)
		{
			return c;
		}

		glm::vec3 n = glm::cross(ab, ac);

		// On edge AB region, return the projection of P onto AB
		float vc = glm::dot(n, glm::cross(-ap, -bp));
		if (vc <= 0.f && s1 >= 0.f && s2 >= 0.f)
		{
			return a + s1 / (s1 + s2) * ab;
		}

		// On edge BC region, return the projection of P onto BC
		float va = glm::dot(n, glm::cross(-bp, -cp));
		if (va <= 0.f && r1 >= 0.f && r2 >= 0.f)
		{
			return b + r1 / (r1 + r2) * bc;
		}

		// On edge AC region, return the projection of P onto AC
		float vb = glm::dot(n, glm::cross(-cp, -ap));
		if (vb <= 0.f && t1 >= 0.f && t2 >= 0.f)
		{
			return a + t1 / (t1 + t2) * ac;
		}

		// The projection of P is inside the triangle
		float abc = vc + va + vb;
		float u = va / abc; // uA
		float v = vb / abc; // vB
		float w = vc / abc; // wC, instead of using w = 1.f - u - v, this line doesn't depend
							// on u and v, somehow the processor can prefetch and excute it.
		return u * a + v * b + w * c;
	}

	glm::vec3 ClosestPointOnTriangle2nd(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
	{
		// Refering "Real-time Collision Dection" by Erison Christer
		// Section 5.1.5
		// Check the Voronoi region of vertices, also applying the Lagrange identity to
		// reduce number of cross product:
		// dot(cross(a, b), cross(c, d)) = dot(a, c) * dot(b, d) - dot(a, d) * dot(b, c).
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;

		// Check if P in vertex region ouside A
		glm::vec3 ap = p - a;

		float d1 = glm::dot(ab, ap);
		float d2 = glm::dot(ac, ap);
		if (d1 <= 0.f && d2 <= 0.f)
		{
			return a;
		}

		// Check if P in vertex region ouside B
		glm::vec3 bp = p - b;
		float d3 = glm::dot(ab, bp);
		float d4 = glm::dot(ac, bp);
		if (d3 >= 0.f && d4 <= d3)
		{
			return b;
		}

		// Check if P in edge region of AB, if so return the projection of P onto AB
		float vc = d1 * d4 - d3 * d2;
		if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f)
		{
			float v = d1 / (d1 - d3);
			return a + v * ab;
		}

		// Check if P in vertex region ouside C
		glm::vec3 cp = p - c;
		float d5 = glm::dot(ab, cp);
		float d6 = glm::dot(ac, cp);
		if (d6 >= 0.f && d5 <= d6)
		{
			return c;
		}

		// Check if P in edge region of AC
		float vb = d5 * d2 - d1 * d6;
		if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f)
		{
			float w = d2 / (d2 - d6);
			return a + w * ac;
		}

		// Check if P in edge region of BC
		float va = d3 * d6 - d5 * d4;
		if (va <= 0.f && d4 >= d3 && d5 >= d6)
		{
			float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
			return b + w * (c - b);
		}

		// P's projection is inside the triangle
		float denom = 1.f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		return a + ab * v + ac * w;
	}

	glm::vec3 ClosestPointOnTriangle(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
	{
		//return ClosestPointOnTriangle1st(p, a, b, c);
		return ClosestPointOnTriangle2nd(p, a, b, c);
	}

	// Return the closest point on AABB or p itself if p is inside the AABB
	glm::vec3 ClosestPointOnAABB(const glm::vec3& p, const glm::vec3& bmin, const glm::vec3& bmax)
	{
		glm::vec3 res = p;

		for (int i = 0; i < 3; ++i)
		{
			if (p[i] < bmin[i])
			{
				res[i] = bmin[i];
			}

			if (p[i] > bmax[i])
			{
				res[i] = bmax[i];
			}
		}

		return res;
	}
} // namespace PotatoEngine
