#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace PotatoEngine
{
	Camera::Camera():
		m_position(0.f, 0.f, 0.f),
		m_forward(0.f, 0.f, -1.f),
		m_right(1.f, 0.f, 0.f)
	{
		// todo: check LH or RH
		m_up = glm::cross(m_forward, m_right);
		m_up = glm::normalize(m_up);
	}

	Camera::Camera(
		const glm::vec3 pos,
		const glm::vec3 forward,
		const glm::vec3 right) :
		m_position(pos),
		m_forward(forward),
		m_right(right)
	{
		// todo: validate up vector
		m_up = glm::cross(m_forward, m_right);
		m_up = glm::normalize(m_up);
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

} // namespace PotatoEngine
