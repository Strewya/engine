#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include "window/window_include.h"
#include <xinput.h>
/******* extra headers *******/
#include "window/window_event.h"
/******* end header inclusion *******/

namespace Core
{
   class GamepadHandler
   {
   public:
      GamepadHandler();

      std::vector<WindowEvent> handle(uint64_t currentTime);

   private:
      static const uint8_t MAX_GAMEPADS = 4;
      
      uint64_t m_unconnectedReadDelay;

      XINPUT_STATE m_gamepadState[MAX_GAMEPADS];
      uint64_t m_gamepadLastUpdateTime[MAX_GAMEPADS];
      bool m_gamepadConnected[MAX_GAMEPADS];
   };
}
