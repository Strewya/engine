#pragma once
/********************************************
*  contents:   main game class that contains game state, resources and systems
*  usage:      
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include "audio/audio_system.h"
#include "games/rainbowland/game_resources.h"
#include "games/rainbowland/game_systems.h"
#include "games/rainbowland/game_state.h"
#include "graphics/graphics_system.h"
#include "input/input_system.h"
#include "util/time/timer.h"
#include "window/window_proxy.h"
/******* end header inclusion *******/

namespace Core
{
   class WindowProxy;

   class Game
   {
   public:
      WindowProxy window;

      AudioSystem audioSystem;
      GraphicsSystem graphicsSystem;
      InputSystem inputSystem;

      GameResources assets;
      GameSystems systems;
      GameState state;

      Timer timer;

      bool init(WindowProxy window);
      bool shutdown();

      bool tickLogic(uint32_t updateTime);
      void tickRender(uint32_t updateTime);
   };
}
