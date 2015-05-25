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
   };

   struct GameSystems;
   struct GameResources;
   struct Timer;

   bool initializeGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets);
   bool updateGameState(float dt_s, uint32_t dt_us, GameState& state, GameSystems& systems, GameResources& assets);
   void renderGameState(float dt, GameState& state, GameSystems& systems, GameResources& assets);
}
