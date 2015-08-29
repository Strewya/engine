#pragma once
/********************************************
*  contents:   handler for all gamepad related functionality
*  usage:   uses xinput
********************************************/
/******* c++ headers *******/
#include <vector>
/******* common headers *******/
#include "utility/types.h"
#include "window/window_include.h"
#include <xinput.h>
/******* extra headers *******/
#include "window/window_message.h"
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
      
      u64 m_unconnectedReadDelay;

      XINPUT_STATE m_gamepadState[MAX_GAMEPADS];
      u64 m_gamepadLastUpdateTime[MAX_GAMEPADS];
      bool m_gamepadConnected[MAX_GAMEPADS];
   };
}
