#pragma once
/********************************************
*  contents:   camera used by the graphics system to render the view of the game
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec3.h"
/******* end header inclusion *******/

namespace core
{
   struct Camera
   {
   public:
      Camera();

      void move(vec3f translation);
      void rotate(vec3f rotation);

      vec3f getPosition() const;
      vec3f getLookAtAxis() const;
      vec3f getUpAxis() const;
      vec3f getRotation() const;
      float getSpeed() const;

      void setPosition(vec3f pos);
      void setUpAxis(vec3f upAxis);
      void setLookAtAxis(vec3f lookAtAxis);
      void setRotation(vec3f rot);
      void setSpeed(float speed);

   private:
      vec3f m_position;
      vec3f m_lookAtAxis;
      vec3f m_upAxis;
      vec3f m_rotation;
      float m_speed;
   };
}
