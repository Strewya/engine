#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/camera.h"
#include "graphics/mesh/mesh.h"
#include "util/rect.h"
#include "util/vec3.h"
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

      Mesh backgroundMesh;
      Mesh playerMesh;
      struct PlayerMovement
      {
         Vec2f position;
         Vec2f direction;
         float currentSpeed;
         float maxSpeed;
         float acceleration;
      };
      PlayerMovement playerMover;
   };

   class GameSystems;
   class GameResources;
   class Timer;

   bool initializeGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets);
   bool updateGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets);
   void renderGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets);
}
