#pragma once
/********************************************
*  contents:   main game class that contains game state, resources and systems
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
   //services
#include "audio/audio_system.h"
#include "graphics/graphics_system.h"
#include "graphics/font/font_system.h"
#include "input/input_system.h"
#include "lua/lua_system.h"
#include "window/window_proxy.h"

#include "games/rainbowland/logic/code.h"
/******* end header inclusion *******/

namespace core
{
   struct Clock;

   struct Game
   {
      WindowProxy window;

      AudioSystem audioSystem;
      GraphicsSystem graphicsSystem;
      InputSystem inputSystem;
      LuaSystem luaSystem;
      FontSystem fontSystem;

      //game state stuff
      GameData game;

      bool isPaused;
      bool init(WindowProxy window);
      bool shutdown();

      bool tickLogic(const Clock& logicClock);
      void tickRender(const Clock& renderClock);
   };
}
