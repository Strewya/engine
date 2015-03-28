#pragma once
/********************************************
*  contents:   Keyboard handler
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
#include <Window/myWindows.h>
/******* extra headers *******/
#include <Window/WindowEvent.h>
/******* end header inclusion *******/

namespace Core
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
