//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "input/input_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "input/gamepad.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "utility/time/clock.h"
#include "utility/communication_buffer.h"
#include "utility/utility.h"
#include "window/window_message.h"
/******* end headers *******/

namespace core
{
   void InputSystem::gatherInputForCurrentFrame(u64 pollMicros, CommunicationBuffer* comm)
   {
      m_inputEvents.clear();
      WinMsg msg{};
      while( comm->peek(msg, pollMicros) )
      {
         m_inputEvents.push_back(msg);
      }
   }

   const EventVector_t& InputSystem::getEvents() const
   {
      return m_inputEvents;
   }
}
