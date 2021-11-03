#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace PotatoEngine
{
	Camera::Camera():
		m_position(0.f, 0.f, 10.f),
		m_forward(0.f, 0.f, -1.f),
		m_right(1.f, 0.f, 0.f),
		m_fovy(glm::radians(90.f))
	{
		// todo: check LH or RH
		m_forward = glm::normalize(m_forward);
		m_up = glm::cross(m_forward, m_right);
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

	const glm::mat4 Camera::GetPerpectiveProjectionMatrix(float aspect) const
	{
		return glm::perspective(m_fovy, aspect, 0.1f, 1000.f);
	}

} // namespace PotatoEngine
