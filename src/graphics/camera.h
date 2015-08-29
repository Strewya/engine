#pragma once
/********************************************
*  contents:   camera used by the graphics system to render the view of the game
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Camera
   {
   public:
      Camera();

      void move(v3 translation);
      void rotate(v3 rotation);

      v3 getPosition() const;
      v3 getLookAtAxis() const;
      v3 getUpAxis() const;
      v3 getRotation() const;
      f32 getSpeed() const;

      void setPosition(v3 pos);
      void setUpAxis(v3 upAxis);
      void setLookAtAxis(v3 lookAtAxis);
      void setRotation(v3 rot);
      void setSpeed(f32 speed);

   private:
      v3 m_position;
      v3 m_lookAtAxis;
      v3 m_upAxis;
      v3 m_rotation;
      f32 m_speed;
   };
}
