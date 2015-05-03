#pragma once
/********************************************
*  contents:   handler for mouse related messages from the OS
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
#include "window/window_include.h"
/******* extra headers *******/
#include "window/window_event.h"
/******* end header inclusion *******/

namespace Core
{
   class MouseHandler
   {
   public:
      MouseHandler();

      void setRelativeMouseMove(bool isRelative, uint32_t centerX, uint32_t centerY);


      bool handle(WindowEvent& we, uint32_t msg, WPARAM wp, LPARAM lp);

   private:
      uint32_t m_centerX;
      uint32_t m_centerY;
      bool m_relativeMouseMove;
   };
}
