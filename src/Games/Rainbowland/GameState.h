#pragma once
/********************************************
*  contents:   GameState
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Game_Gameplay.h>
#include <Graphics/Camera.h>
#include <Util/Vec3.h>
/******* end header inclusion *******/

namespace Core
{
   class GameState
   {
   public:
      enum class State
      {
         ColorPick,
         ClassPick,
         Session
      };
      State state;
      Camera camera;
      Vec3f moveDirection;
      Gameplay gameplay;
   };

   class GameSystems;
   class GameResources;

   void updateGame(GameState& state, GameSystems& systems, GameResources& assets);
   void renderGame(GameState& state, GameSystems& systems, GameResources& assets);
}
