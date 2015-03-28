#pragma once
/********************************************
*  contents:   Gamepad handler
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include <Window/myWindows.h>
#include <XInput.h>
/******* extra headers *******/
#include <Window/WindowEvent.h>
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
