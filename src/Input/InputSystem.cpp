//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Input/InputSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Input/Gamepad.h>
#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <Util/Clock.h>
#include <Util/Utility.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
   bool InputSystem::init(Window& window)
   {
      CORE_INIT_START(InputSystem);

      m_window = &window;

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
      m_inputEvents = m_window->collectEvents(currentPeekTime);
   }

   EventVector_t InputSystem::getEvents() const
   {
      return m_inputEvents;
   }

   uint32_t InputSystem::getEventCount() const
   {
      return m_inputEvents.size();
   }
}
