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
#include "window/window_message.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;

   struct MouseHandler
   {
   public:
      MouseHandler();

      void setRelativeMouseMove(bool isRelative, u32 centerX, u32 centerY);


      void handle(CommunicationBuffer* buffer, u32 msg, WPARAM wp, LPARAM lp);

   private:
      u32 m_centerX;
      u32 m_centerY;
      bool m_relativeMouseMove;
   };
}
