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

   void Camera::move(v3 translation)
   {
      m_position += translation;
   }

   void Camera::rotate(v3 rotation)
   {
      m_rotation += rotation;
   }

   v3 Camera::getPosition() const
   {
      return m_position;
   }

   v3 Camera::getLookAtAxis() const
   {
      return m_lookAtAxis;
   }

   v3 Camera::getUpAxis() const
   {
      return m_upAxis;
   }

   v3 Camera::getRotation() const
   {
      return m_rotation;
   }

   f32 Camera::getSpeed() const
   {
      return m_speed;
   }

   void Camera::setPosition(v3 pos)
   {
      m_position = pos;
   }

   void Camera::setUpAxis(v3 upAxis)
   {
      m_upAxis = upAxis;
   }

   void Camera::setLookAtAxis(v3 lookAtAxis)
   {
      m_lookAtAxis = lookAtAxis;
   }

   void Camera::setRotation(v3 rot)
   {
      m_rotation = rot;
   }

   void Camera::setSpeed(f32 speed)
   {
      m_speed = speed;
   }
}
