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
#include "util/types.h"
#include "window/window_event.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;

   struct KeyboardHandler
   {
   public:
      KeyboardHandler();

      void handle(CommunicationBuffer* buffer, u32 msg, WPARAM wp, LPARAM lp);

   private:
      enum
      {
         MAX_KEYS = 255
      };
      
      i8 m_keyCodes[MAX_KEYS];
   };
}
