//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/state/main_menu.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/resources.h"
#include "games/rainbowland/systems.h"
/******* end headers *******/

namespace core
{
   //*****************************************************************
   //          MAIN MENU INIT
   //*****************************************************************
   bool mainMenu_init(MainMenuState& state, GameSystems systems, GameResources& assets)
   {
      return true;
   }
   
   //*****************************************************************
   //          MAIN MENU UPDATE
   //*****************************************************************
   bool mainMenu_update(float dt, uint32_t deltaMicros, MainMenuState& state, GameSystems systems, GameResources& assets)
   {
      return true;
   }
   
   //*****************************************************************
   //          MAIN MENU RENDER
   //*****************************************************************
   void mainMenu_render(float dt, MainMenuState& state, GameSystems systems, GameResources& assets)
   {

   }
}