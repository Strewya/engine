#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Clock;
   class Window;
   class WindowEvent;

   typedef std::vector<WindowEvent> EventVector_t;

   class InputSystem
   {
   public:
      bool init(Window& window);
      bool shutdown();

      void update();

      EventVector_t getEvents() const;
      uint32_t getEventCount() const;

   private:
      Window* m_window;
      EventVector_t m_inputEvents;
   };
}
