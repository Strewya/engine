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
#include "utility/array.h"
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
         u32 writeIndex = m_writeIndex;
         if( canWrite(writeIndex) )
         {
            writeIndex = writeIndex % m_maxMessages;
            m_buffer[writeIndex] = e;
            m_buffer[writeIndex].timestamp = Clock::getRealTimeMicros();
            ++m_writeIndex;
         }
         else
         {
            CORE_LOG("Communication buffer is full, skipping event [type=", (u32)e.type, "]");
         }
      }

      bool peek(WinMsg& e)
      {
         u32 readIndex = m_readIndex;
         if( canRead(readIndex) )
         {
            readIndex = (readIndex + 1) % m_maxMessages;
            e = m_buffer[readIndex];
            memset(&m_buffer[readIndex], 0, sizeof(WinMsg));
            ++m_readIndex;
            return true;
         }
         return false;
      }

      bool canRead(u32 index) const
      {
         auto empty = m_writeIndex == (index + 1);
         return !empty;
      }

      bool canWrite(u32 index) const
      {
         auto full = (m_readIndex == index);
         return !full;
      }

      Array<WinMsg> getMessagesUntil(u64 timepoint) const
      {
         u32 readIndex = m_readIndex;
         Array<WinMsg> result{m_buffer + readIndex + 1, 0};
         while( canRead(readIndex) && (result.data + result.count)->timestamp <= timepoint )
         {
            ++result.count;
            ++readIndex;
         }
         return result;
      }

      void consumeMessages(u32 count)
      {
         memset(m_buffer + m_readIndex + 1, 0, sizeof(WinMsg)*count);
         m_readIndex += count;
      }
   };
}
