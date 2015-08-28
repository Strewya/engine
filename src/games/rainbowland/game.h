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

#include "games/rainbowland/logic/code.h"
/******* end header inclusion *******/

namespace core
{
   struct Clock;
   struct CommunicationBuffer;

   struct Game
   {
      CommunicationBuffer* toMain;
      CommunicationBuffer* fromMain;

      AudioSystem audioSystem;
      GraphicsSystem graphicsSystem;
      InputSystem inputSystem;
      LuaSystem luaSystem;
      FontSystem fontSystem;

      //game state stuff
      GameData game;

      bool init(CommunicationBuffer* fromMain, CommunicationBuffer* toMain);
      bool isPaused;
      bool shutdown();

      void onGainFocus();
      void onLostFocus();

      bool tickLogic(const Clock& logicClock);
      void tickRender(const Clock& renderClock);
   };
}
