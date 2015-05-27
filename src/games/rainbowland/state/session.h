#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/rect.h"
/******* end header inclusion *******/

namespace core
{
   struct GameResources;
   struct GameSystems;
   struct Timer;

   struct SessionState
   {
      Rect square;
   };

   bool session_init(SessionState& state, GameSystems systems, GameResources& assets);
   bool session_update(float dt, uint32_t deltaMicros, SessionState& totalGameState, GameSystems systems, GameResources& assets);
   void session_render(float dt, SessionState& totalGameState, GameSystems systems, GameResources& assets);
}