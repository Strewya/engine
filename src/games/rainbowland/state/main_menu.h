#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct GameResources;
   struct GameSystems;
   struct Timer;

   struct MainMenuState
   {
      
   };

   bool mainMenu_init(MainMenuState& state, GameSystems services, GameResources& assets);
   bool mainMenu_update(float dt, uint32_t deltaMicros, MainMenuState& state, GameSystems services, GameResources& assets);
   void mainMenu_render(float dt, MainMenuState& state, GameSystems services, GameResources& assets);
}