//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Camera.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Camera::Camera()
		: m_position(0, 0, 0), m_lookAtAxis(0, 0, 1), m_upAxis(0, 1, 0), m_speed(0)
	{}

	void Camera::move(const Vec3& translation)
	{
		m_position += translation;
	}

	void Camera::rotate(const Vec3& rotation)
	{
		m_rotation += rotation;
	}

	const Vec3& Camera::getPosition() const
	{
		return m_position;
	}

	const Vec3& Camera::getLookAtAxis() const
	{
		return m_lookAtAxis;
	}

	const Vec3& Camera::getUpAxis() const
	{
		return m_upAxis;
	}

	const Vec3& Camera::getRotation() const
	{
		return m_rotation;
	}

	float Camera::getSpeed() const
	{
		return m_speed;
	}

	void Camera::setPosition(const Vec3& pos)
	{
		m_position = pos;
	}

	void Camera::setUpAxis(const Vec3& upAxis)
	{
		m_upAxis = upAxis;
	}

	void Camera::setLookAtAxis(const Vec3& lookAtAxis)
	{
		m_lookAtAxis = lookAtAxis;
	}

	void Camera::setRotation(const Vec3& rot)
	{
		m_rotation = rot;
	}

	void Camera::setSpeed(float speed)
	{
		m_speed = speed;
	}
}
