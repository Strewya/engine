//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/camera.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/geometry/vec3.h"
/******* end headers *******/

namespace core
{
   Camera::Camera()
      : m_position({0, 0, 0}), m_lookAtAxis({0, 0, 1}), m_upAxis({0, 1, 0}), m_speed(0)
   {
   }

   void Camera::move(Vec3 translation)
   {
      m_position += translation;
   }

   void Camera::rotate(Vec3 rotation)
   {
      m_rotation += rotation;
   }

   Vec3 Camera::getPosition() const
   {
      return m_position;
   }

   Vec3 Camera::getLookAtAxis() const
   {
      return m_lookAtAxis;
   }

   Vec3 Camera::getUpAxis() const
   {
      return m_upAxis;
   }

   Vec3 Camera::getRotation() const
   {
      return m_rotation;
   }

   float Camera::getSpeed() const
   {
      return m_speed;
   }

   void Camera::setPosition(Vec3 pos)
   {
      m_position = pos;
   }

   void Camera::setUpAxis(Vec3 upAxis)
   {
      m_upAxis = upAxis;
   }

   void Camera::setLookAtAxis(Vec3 lookAtAxis)
   {
      m_lookAtAxis = lookAtAxis;
   }

   void Camera::setRotation(Vec3 rot)
   {
      m_rotation = rot;
   }

   void Camera::setSpeed(float speed)
   {
      m_speed = speed;
   }
}
