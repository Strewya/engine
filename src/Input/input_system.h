#pragma once
/********************************************
*  contents:   input system
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include "window/window_proxy.h"
/******* end header inclusion *******/

namespace Core
{
   struct Clock;
   struct WindowEvent;

   typedef std::vector<WindowEvent> EventVector_t;

   struct InputSystem
   {
   public:
      bool init(WindowProxy window);
      bool shutdown();

      void update();

      EventVector_t getEvents() const;

   private:
      WindowProxy m_window;
      EventVector_t m_inputEvents;
   };
}
