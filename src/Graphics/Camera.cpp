//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/camera.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Camera::Camera()
      : m_position(0, 0, 0), m_lookAtAxis(0, 0, 1), m_upAxis(0, 1, 0), m_speed(0)
   {
   }

   void Camera::move(Vec3f translation)
   {
      m_position += translation;
   }

   void Camera::rotate(Vec3f rotation)
   {
      m_rotation += rotation;
   }

   Vec3f Camera::getPosition() const
   {
      return m_position;
   }

   Vec3f Camera::getLookAtAxis() const
   {
      return m_lookAtAxis;
   }

   Vec3f Camera::getUpAxis() const
   {
      return m_upAxis;
   }

   Vec3f Camera::getRotation() const
   {
      return m_rotation;
   }

   float Camera::getSpeed() const
   {
      return m_speed;
   }

   void Camera::setPosition(Vec3f pos)
   {
      m_position = pos;
   }

   void Camera::setUpAxis(Vec3f upAxis)
   {
      m_upAxis = upAxis;
   }

   void Camera::setLookAtAxis(Vec3f lookAtAxis)
   {
      m_lookAtAxis = lookAtAxis;
   }

   void Camera::setRotation(Vec3f rot)
   {
      m_rotation = rot;
   }

   void Camera::setSpeed(float speed)
   {
      m_speed = speed;
   }
}
