#pragma once
/********************************************
*  contents:   handler for keyboard related messages from the OS
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
#include "window/window_include.h"
/******* extra headers *******/
#include "window/window_event.h"
/******* end header inclusion *******/

namespace core
{
   class KeyboardHandler
   {
   public:
      KeyboardHandler();

      bool handle(WindowEvent& we, uint32_t msg, WPARAM wp, LPARAM lp);

   private:
      static const uint32_t MAX_KEYS = 255;
      
      int8_t m_keyCodes[MAX_KEYS];
   };
}
