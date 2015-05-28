#pragma once
/********************************************
*  contents:   main storage for game state
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/camera.h"
#include "graphics/font/font_descriptor.h"

#include "games/rainbowland/state/main_menu.h"
#include "games/rainbowland/state/session.h"
/******* end header inclusion *******/

namespace core
{
   struct GameState
   {
      enum class GlobalGameState
      {
         MainMenu,
         Gameplay,
         Score
      };

      enum class GameplayState
      {
         //ColorPick,
         ClassPick,
         Session
      };

      GlobalGameState globalGameState;
      GameplayState gameplayState;
      Camera camera;
      // #test
      FontDescriptor fontDesc;

      MainMenuState mainMenuState;
      SessionState sessionState;
   };

   struct GameSystems;
   struct GameResources;

   bool game_init(GameState& state, GameSystems services, GameResources& assets);
   bool game_update(float dt, uint32_t deltaMicros, GameState& state, GameSystems services, GameResources& assets);
   void game_render(float dt, GameState& state, GameSystems services, GameResources& assets);
}
