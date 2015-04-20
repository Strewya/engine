#pragma once
/********************************************
*  class:   Game
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <Audio/AudioSystem.h>
#include <Games/Rainbowland/GameResources.h>
#include <Games/Rainbowland/GameSystems.h>
#include <Graphics/GraphicsSystem.h>
#include <Input/InputSystem.h>
#include <Games/Rainbowland/GameState.h>
#include <Util/Timer.h>
/******* end header inclusion *******/

namespace Core
{
   class Window;

   class Game
   {
   public:
      Window* window;

      AudioSystem audioSystem;
      GraphicsSystem graphicsSystem;
      InputSystem inputSystem;

      GameResources assets;
      GameSystems systems;
      GameState state;

      Timer timer;

      bool init(Window& window);
      bool shutdown();

      bool tickLogic(uint32_t updateTime);
      void tickRender(uint32_t updateTime);
   };
}
