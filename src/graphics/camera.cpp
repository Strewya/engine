//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/camera.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Camera::Camera()
      : m_position({0, 0, 0}), m_lookAtAxis({0, 0, 1}), m_upAxis({0, 1, 0}), m_speed(0)
   {
   }

   void Camera::move(vec3f translation)
   {
      m_position += translation;
   }

   void Camera::rotate(vec3f rotation)
   {
      m_rotation += rotation;
   }

   vec3f Camera::getPosition() const
   {
      return m_position;
   }

   vec3f Camera::getLookAtAxis() const
   {
      return m_lookAtAxis;
   }

   vec3f Camera::getUpAxis() const
   {
      return m_upAxis;
   }

   vec3f Camera::getRotation() const
   {
      return m_rotation;
   }

   float Camera::getSpeed() const
   {
      return m_speed;
   }

   void Camera::setPosition(vec3f pos)
   {
      m_position = pos;
   }

   void Camera::setUpAxis(vec3f upAxis)
   {
      m_upAxis = upAxis;
   }

   void Camera::setLookAtAxis(vec3f lookAtAxis)
   {
      m_lookAtAxis = lookAtAxis;
   }

   void Camera::setRotation(vec3f rot)
   {
      m_rotation = rot;
   }

   void Camera::setSpeed(float speed)
   {
      m_speed = speed;
   }
}
