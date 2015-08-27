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
#include "util/time/clock.h"
#include "util/utility.h"
#include "window/window_event.h"
/******* end headers *******/

namespace core
{
   bool InputSystem::init(WindowProxy window)
   {
      CORE_INIT_START(InputSystem);

      m_window = window;

      CORE_INIT_END(InputSystem);
   }

   bool InputSystem::shutdown()
   {
      CORE_SHUTDOWN_START(InputSystem);

      CORE_SHUTDOWN_END(InputSystem);
   }

   void InputSystem::gatherInputForCurrentFrame(uint64_t pollMicros)
   {
      m_inputEvents.clear();
      m_inputEvents = m_window.collectEvents(pollMicros);
   }

   const EventVector_t& InputSystem::getEvents() const
   {
      return m_inputEvents;
   }
}
