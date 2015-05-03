#pragma once
/********************************************
*  contents:   main storage for game state
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/camera.h"
#include "graphics/mesh/mesh.h"
#include "util/geometry/rect.h"
#include "util/geometry/vec3.h"
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
         float acceleration;
         float currentSpeed;
         float maxSpeed;
         Vec2f direction;
         Vec2f position;
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
