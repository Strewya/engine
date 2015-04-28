#pragma once
/********************************************
*  contents:   #todo
*  usage:      
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <audio/audio_system.h>
#include <games/rainbowland/game_resources.h>
#include <games/rainbowland/game_systems.h>
#include <games/rainbowland/game_state.h>
#include <graphics/graphics_system.h>
#include <input/input_system.h>
#include <util/timer.h>
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
