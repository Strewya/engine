#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <array>
#include <atomic>
/******* common headers *******/
/******* extra headers *******/
#include "util/types.h"
#include "util/utility.h"
#include "util/time/clock.h"
#include "window/window_event.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer
   {
   public:
      void writeEvent(const WindowEvent& e)
      {
         if( !full )
         {
            buffer[writeIndex] = e;
            buffer[writeIndex].timestamp = Clock::getRealTimeMicros();
            ++writeIndex;
            if( writeIndex == readIndex )
            {
               full = true;
            }
         }
         else
         {
            CORE_LOG("Communication buffer is full, skipping event [type=", e.type, "]");
         }
      }

      bool peekEvent(WindowEvent& e)
      {
         if( !empty() )
         {
            e = buffer[readIndex];
            memset(&buffer[readIndex], 0, sizeof(WindowEvent));
            ++readIndex;
            if( full )
            {
               full = false;
            }
            return true;
         }
         return false;
      }

      bool empty() const
      {
         auto result = full || (readIndex == writeIndex);
         return result;
      }

   private:
      enum
      {
         MAX_MESSAGES = 1024
      };
      std::array<core::WindowEvent, MAX_MESSAGES> buffer;
      std::atomic<u32> readIndex;
      std::atomic<u32> writeIndex;
      std::atomic<bool> full;
   };

}
