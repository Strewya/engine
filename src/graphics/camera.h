#pragma once
/********************************************
*  contents:   camera used by the graphics system to render the view of the game
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "utility/geometry/vec_types.h"
/******* end header inclusion *******/

namespace core
{
   struct Camera
   {
   public:
      Camera();

      void move(Vec3 translation);
      void rotate(Vec3 rotation);

      Vec3 getPosition() const;
      Vec3 getLookAtAxis() const;
      Vec3 getUpAxis() const;
      Vec3 getRotation() const;
      float getSpeed() const;

      void setPosition(Vec3 pos);
      void setUpAxis(Vec3 upAxis);
      void setLookAtAxis(Vec3 lookAtAxis);
      void setRotation(Vec3 rot);
      void setSpeed(float speed);

   private:
      Vec3 m_position;
      Vec3 m_lookAtAxis;
      Vec3 m_upAxis;
      Vec3 m_rotation;
      float m_speed;
   };
}
