#pragma once
/********************************************
*  contents:   Movement code
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   struct Movable
   {
      Vec2f position;
      Vec2f direction;
      float speed;
      float maxSpeed;
      float acceleration;
      float maxAcceleration;
      
   };

   typedef std::vector<Movable> VMovables;

   void processMovement(VMovables& list);
}
