#pragma once
/********************************************
*  contents:   GameState
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Camera.h>
#include <Util/Vec3.h>
/******* end header inclusion *******/

namespace Core
{
   class GameState
   {
   public:
      Camera camera;
      Vec3f moveDirection;
   };
}
