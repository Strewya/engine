#pragma once
/********************************************
*  class:   Camera
*  usage:
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

      void move(Vec3f translation);
      void rotate(Vec3f rotation);

      Vec3f getPosition() const;
      Vec3f getLookAtAxis() const;
      Vec3f getUpAxis() const;
      Vec3f getRotation() const;
      float getSpeed() const;

      void setPosition(Vec3f pos);
      void setUpAxis(Vec3f upAxis);
      void setLookAtAxis(Vec3f lookAtAxis);
      void setRotation(Vec3f rot);
      void setSpeed(float speed);

   private:
      Vec3f m_position;
      Vec3f m_lookAtAxis;
      Vec3f m_upAxis;
      Vec3f m_rotation;
      float m_speed;
   };
}
