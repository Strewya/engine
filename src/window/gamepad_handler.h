#pragma once
/********************************************
*  contents:   handler for all gamepad related functionality
*  usage:   uses xinput
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include "window/window_include.h"
#include <xinput.h>
/******* extra headers *******/
#include "util/types.h"
#include "window/window_event.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;

   struct  GamepadHandler
   {
   public:
      GamepadHandler();

      void handle(CommunicationBuffer* buffer, u64 currentTime);

   private:
      enum
      {
         MAX_GAMEPADS = 4,
      };
      
      uint64_t m_unconnectedReadDelay;

      XINPUT_STATE m_gamepadState[MAX_GAMEPADS];
      uint64_t m_gamepadLastUpdateTime[MAX_GAMEPADS];
      bool m_gamepadConnected[MAX_GAMEPADS];
   };
}
