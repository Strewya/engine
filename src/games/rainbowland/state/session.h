#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct GameResources;
   struct GameSystems;


   struct SessionState
   {
      
   };

   bool session_init(SessionState& state, GameSystems services, GameResources& assets);
   bool session_update(float dt, uint32_t deltaMicros, SessionState& state, GameSystems services, GameResources& assets);
   void session_render(float dt, SessionState& state, GameSystems services, GameResources& assets);
}