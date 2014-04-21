#pragma once
/********************************************
*	class:	Camera
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec3.h>
/******* end header inclusion *******/

namespace Core
{
	class Camera
	{
	public:
		Camera();

		void move(const Vec3& translation);
		void rotate(const Vec3& rotation);

		const Vec3& getPosition() const;
		const Vec3& getLookAtAxis() const;
		const Vec3& getUpAxis() const;
		const Vec3& getRotation() const;
		float getSpeed() const;

		void setPosition(const Vec3& pos);
		void setUpAxis(const Vec3& upAxis);
		void setLookAtAxis(const Vec3& lookAtAxis);
		void setRotation(const Vec3& rot);
		void setSpeed(float speed);

	private:
		Vec3 m_position;
		Vec3 m_lookAtAxis;
		Vec3 m_upAxis;
		Vec3 m_rotation;
		float m_speed;
	};
}
