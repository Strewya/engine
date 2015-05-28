#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "games/rainbowland/types/movement.h"
#include "util/geometry/circle.h"
#include "util/geometry/vec2.h"
/******* end header inclusion *******/

namespace core
{
   struct GameResources;
   struct GameSystems;
   struct Timer;


   struct SessionState
   {
      
   };

   bool session_init(SessionState& state, GameSystems systems, GameResources& assets);
   bool session_update(float dt, uint32_t deltaMicros, SessionState& state, GameSystems systems, GameResources& assets);
   void session_render(float dt, SessionState& state, GameSystems systems, GameResources& assets);
}