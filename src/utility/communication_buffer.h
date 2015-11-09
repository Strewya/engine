#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <array>
#include <atomic>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/memory.h"
#include "utility/utility.h"
#include "utility/time/clock.h"
#include "window/window_message.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer
   {
   private:
      std::atomic<u32> m_readIndex;
      std::atomic<u32> m_writeIndex;
      u32 m_maxMessages;
      WinMsg* m_buffer;

   public:
      void init(Memory& memory, u32 messageCount)
      {
         m_writeIndex = 1;
         m_readIndex = 0;
         m_maxMessages = messageCount;
         m_buffer = emplaceArray<WinMsg>(memory, messageCount);
         CORE_ASSERT_DBGERR(m_buffer != nullptr, "Not enough memory to emplace CommunicationBuffer storage.");
      }

      void writeEvent(const WinMsg& e)
      {
         if( !full() )
         {
            auto index = m_writeIndex % m_maxMessages;
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
            auto index = (m_readIndex + 1) % m_maxMessages;
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

      //blocking call, use only in rare situations
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

      bool isReadableIndex(u32 index) const
      {
         auto empty = m_writeIndex == (index + 1);
         return !empty;
      }

      ReadOnlyArray<WinMsg> getMessagesUntil(u64 timepoint) const
      {
         ReadOnlyArray<WinMsg> result{m_buffer, 0};
         u32 readIndex = m_readIndex;
         while( isReadableIndex(readIndex) && (result.data + result.count)->timestamp <= timepoint )
         {
            ++result.count;
            ++readIndex;
         }
         return result;
      }

      void consumeMessages(u32 count)
      {
         m_readIndex += count;
      }
   };
}
