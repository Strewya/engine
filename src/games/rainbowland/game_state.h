#pragma once
/********************************************
*  contents:   main storage for game state
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "games/rainbowland/types/types.h"
#include "graphics/camera.h"
#include "graphics/mesh/mesh.h"
#include "util/geometry/rect.h"
#include "util/geometry/vec3.h"
#include "util/transform.h"
/******* end header inclusion *******/

namespace Core
{
   struct GameState
   {
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

      std::vector<Transform> transforms;
      std::vector<MovingThing> movingThings;
      std::vector<MoveDirectionTarget> moveDirections;
      std::vector<AimDirection> aimDirections;
      std::vector<Window2GameEvent> inputTranslation;
   };

   struct GameSystems;
   struct GameResources;
   struct Timer;

   bool initializeGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets);
   bool updateGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets);
   void renderGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets);
}
