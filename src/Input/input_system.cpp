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
#include "util/clock.h"
#include "util/utility.h"
#include "window/window_event.h"
/******* end headers *******/

namespace Core
{
   bool InputSystem::init(WindowProxy window)
   {
      CORE_INIT_START(InputSystem);

      m_window = window;

      Gamepad::Init();
      Keyboard::Init();
      Mouse::Init();

      CORE_INIT_END(InputSystem);
   }

   bool InputSystem::shutdown()
   {
      CORE_SHUTDOWN_START(InputSystem);

      CORE_SHUTDOWN_END(InputSystem);
   }

   void InputSystem::update()
   {
      m_inputEvents.clear();
      auto currentPeekTime = Clock::getRealTimeMicros();
      m_inputEvents = m_window.collectEvents(currentPeekTime);
   }

   EventVector_t InputSystem::getEvents() const
   {
      return m_inputEvents;
   }
}
