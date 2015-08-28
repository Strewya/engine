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
#include "utility/types.h"
#include "utility/utility.h"
#include "utility/time/clock.h"
#include "window/window_message.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer
   {
   public:
      void init()
      {
         m_writeIndex = 1;
      }

      void writeEvent(const WinMsg& e)
      {
         if( !full() )
         {
            auto index = m_writeIndex % MAX_MESSAGES;
            m_buffer[index] = e;
            m_buffer[index].timestamp = Clock::getRealTimeMicros();
            ++m_writeIndex;
         }
         else
         {
            CORE_LOG("Communication buffer is full, skipping event [type=", (u32)e.type, "]");
         }
      }

      bool peek(WinMsg& e, u64 latestTime = 0)
      {
         if( !empty() )
         {
            auto index = (m_readIndex + 1) % MAX_MESSAGES;
            if( latestTime == 0 || m_buffer[index].timestamp <= latestTime )
            {
               e = m_buffer[index];
               memset(&m_buffer[index], 0, sizeof(WinMsg));
               ++m_readIndex;
               return true;
            }
         }
         return false;
      }

      WinMsg wait()
      {
         WinMsg msg{};
         while( peek(msg) == false ) {}
         return msg;
      }

      bool empty() const
      {
         auto result = m_writeIndex == (m_readIndex + 1);
         return result;
      }

      bool full() const
      {
         auto result = m_readIndex == m_writeIndex;
         return result;
      }

   private:
      enum
      {
         MAX_MESSAGES = 1024
      };
      std::array<core::WinMsg, MAX_MESSAGES> m_buffer;
      std::atomic<u32> m_readIndex;
      std::atomic<u32> m_writeIndex;
   };

}
