#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace PotatoEngine
{
	Camera::Camera():
		m_position(0.f, 0.f, 1.f),
		m_forward(0.f, 0.f, -1.f),
		m_right(1.f, 0.f, 0.f),
		m_fovy(glm::radians(90.f)),
		m_yaw(-90.f),
		m_pitch(0.0f)
	{
		// right hand coordinate?
		m_forward = glm::normalize(m_forward);
		m_up = glm::cross(m_right, m_forward);
		m_up = glm::normalize(m_up);
	}

	Camera::Camera(
		const glm::vec3 pos,
		const glm::vec3 forward,
		const glm::vec3 right,
		const float fovy_degree) :
		m_position(pos),
		m_forward(forward),
		m_right(right),
		m_fovy(glm::radians(fovy_degree))
	{
		// todo: validate up vector
		m_up = glm::cross(m_right, m_forward);
		m_up = glm::normalize(m_up);
		m_right = glm::cross(m_forward, m_up);

		UpdateYawPitch();
	}

	Camera::~Camera()
	{}


	const glm::vec3& Camera::Position() const
	{
		return m_position;
	}

	const glm::vec3& Camera::Forward() const
	{
		return m_forward;
	}

	const glm::vec3& Camera::Right() const
	{
		return m_right;
	}

	const glm::mat4 Camera::GetViewingMatrix() const
	{
		return glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	const glm::mat4 Camera::GetPerpectiveProjectionMatrix(float aspect) const
	{
		return glm::perspective(m_fovy, aspect, 0.001f, 1000.f);
	}

	void Camera::ProcessMovement(Camera::Movement direction, float deltaTime)
	{
		float distance = m_moveSpeed * deltaTime;

		switch (direction)
		{
		case Movement::FORWARD:
			m_position += m_forward * distance;
			break;
		case Movement::BACKWARD:
			m_position -= m_forward * distance;
			break;
		case Movement::LEFT:
			m_position -= m_right * distance;
			break;
		case Movement::RIGHT:
			m_position += m_right * distance;
			break;
		}
	}

	void Camera::ProcessMouseRotation(float xOffset, float yOffset, bool bConstraintPitch)
	{
		float mouseSensity = 0.5f; // hardcode

		xOffset *= mouseSensity;
		yOffset *= mouseSensity;

		m_yaw += xOffset;
		m_pitch += yOffset;

		if (bConstraintPitch)
		{
			m_pitch = glm::clamp(m_pitch, -89.f, 89.f);
		}

		UpdateDirectionVectors();
	}

	void Camera::UpdateYawPitch()
	{
		float pitchRad = glm::asin(m_forward.y);
		float cosPitch = glm::cos(pitchRad);
		float yawRad = glm::acos(m_forward.x / cosPitch);

		m_pitch = glm::degrees(pitchRad);
		m_yaw = glm::degrees(yawRad);
	}

	void Camera::UpdateDirectionVectors()
	{
		float pitchRad = glm::radians(m_pitch);
		float yawRad = glm::radians(m_yaw);

		glm::vec3 forward;
		forward.x = glm::cos(pitchRad) * glm::cos(yawRad);
		forward.y = glm::sin(pitchRad);
		forward.z = glm::cos(pitchRad) * glm::sin(yawRad);

		m_forward = glm::normalize(forward);
		m_right   = glm::normalize(glm::cross(m_forward, glm::vec3(0.f, 1.f, 0.f)));
		m_up      = glm::normalize(glm::cross(m_right, m_forward));
	}

} // namespace PotatoEngine
