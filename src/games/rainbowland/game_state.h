#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <graphics/camera.h>
#include <graphics/mesh/mesh.h>
#include <util/rect.h>
#include <util/vec3.h>
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
      State activeState;
      Camera camera;
      Vec3f cameraMoveDirection;
      float cameraMoveModifier;

      Mesh background;
      Mesh player;

   };

   class GameSystems;
   class GameResources;

   void initializeGameState(GameState& state, GameSystems& systems, GameResources& assets);
   void updateGameState(GameState& state, GameSystems& systems, GameResources& assets);
   void renderGameState(GameState& state, GameSystems& systems, GameResources& assets);
}
