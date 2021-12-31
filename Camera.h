#ifndef POTATOENGINE_CAMERA_H_
#define POTATOENGINE_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace PotatoEngine
{
	class Camera
	{
	public:
		enum class Movement
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};

		Camera();
		Camera(const glm::vec3 pos, const glm::vec3 forward, const glm::vec3 right, const float fovy_degree);
		~Camera();

		const glm::vec3& Position() const;
		const glm::vec3& Forward() const;
		const glm::vec3& Right() const;

		const glm::mat4 GetViewingMatrix() const;
		const glm::mat4 GetPerpectiveProjectionMatrix(float aspect) const;

		void ProcessMovement(Movement direction, float deltaTime);
		void ProcessMouseRotation(float xOffset, float yOffset, bool bConstraintPitch = true);

	private:
		void UpdateYawPitch();
		void UpdateDirectionVectors();

		glm::vec3 m_position;

		glm::vec3 m_forward;
		glm::vec3 m_right;
		glm::vec3 m_up;

		// euler angles
		float m_yaw;
		float m_pitch;

		float m_fovy; // radian
	}; // class Camera
} // namespace PotatoEngine

#endif // POTATOENGINE_CAMERA_H_
