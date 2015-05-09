//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/ring_buffer.h"
/******* c++ headers *******/
#include <cassert>
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   RingBuffer::RingBuffer(int32_t initialSize, int32_t increment)
      : m_nextWriteIndex(0), m_nextReadIndex(0), m_increment(increment)
   {
      m_data.resize(initialSize);
   }

   void RingBuffer::init(int32_t initialSize, int32_t increment)
   {
      m_increment = increment;
      m_data.resize(initialSize);
   }

   void RingBuffer::push_back(uint32_t value)
   {
      m_data[m_nextWriteIndex++] = value;

      if( m_nextWriteIndex == m_nextReadIndex )
      {
         std::rotate(m_data.begin(), m_data.begin() + m_nextReadIndex, m_data.end());
         m_nextWriteIndex += std::distance(m_data.begin() + m_nextReadIndex, m_data.end());
         m_nextReadIndex = 0;
      }

      if( m_nextWriteIndex == m_data.size() )
      {
         if( m_nextReadIndex == 0 )
         {
            m_data.resize(m_data.size() * 2);
         }
         else
         {
            m_nextWriteIndex = 0;
         }
      }
   }

   void RingBuffer::pop_front()
   {
      m_nextReadIndex = (++m_nextReadIndex) % m_data.size();
      if( m_nextReadIndex == m_nextWriteIndex )
      {
         m_nextReadIndex = m_nextWriteIndex = 0;
      }
   }

   void RingBuffer::clear()
   {
      m_nextReadIndex = m_nextWriteIndex = 0;
   }

   bool RingBuffer::empty() const
   {
      auto result = m_nextReadIndex == m_nextWriteIndex;

      return result;
   }

   uint32_t RingBuffer::front() const
   {
      uint32_t result{};
      if( !empty() )
      {
         result = m_data[m_nextReadIndex];
      }

      return result;
   }

   uint32_t RingBuffer::size() const
   {
      auto result = m_nextWriteIndex >= m_nextReadIndex ?
         m_nextWriteIndex - m_nextReadIndex :
         m_data.size() + m_nextWriteIndex - m_nextReadIndex;

      return result;
   }

   void test_ring_buffer()
   {
      {
         RingBuffer rb;
         assert(rb.empty() == true);
         assert(rb.size() == 0);

         rb.push_back(1);
         //1 0 0 0
         //R W
         assert(rb.size() == 1);

         rb.push_back(2);
         //1 2 0 0
         //R   W
         assert(rb.size() == 2);

         rb.push_back(3);
         //1 2 3 0
         //R     W
         assert(rb.size() == 3);

         rb.push_back(4);
         //1 2 3 4 0 0 0 0
         //R       W
         assert(rb.size() == 4);

         rb.push_back(5);
         //1 2 3 4 5 0 0 0
         //R         W
         assert(rb.size() == 5);

         assert(rb.empty() == false);
         rb.clear();
         assert(rb.empty() == true);
         assert(rb.size() == 0);
      }
      {
         RingBuffer rb;
         //0 0 0 0
         //RW
         assert(rb.size() == 0);

         rb.push_back(1);
         //1 0 0 0
         //R W
         assert(rb.size() == 1);

         rb.push_back(2);
         //1 2 0 0
         //R   W
         assert(rb.size() == 2);

         rb.push_back(3);
         //1 2 3 0
         //R     W
         assert(rb.size() == 3);

         assert(rb.front() == 1);
         rb.pop_front();
         //1 2 3 0
         //  R   W
         assert(rb.size() == 2);
         
         assert(rb.front() == 2);
         rb.pop_front();
         //1 2 3 0
         //    R W
         assert(rb.size() == 1);
         
         rb.push_back(1);
         //1 2 3 1
         //W   R
         assert(rb.size() == 2);

         rb.push_back(6);
         //6 2 3 1
         //  W R
         assert(rb.size() == 3);

         rb.push_back(7);
         //6 7 3 1
         //    RW
         //3 1 6 7 0 0 0 0
         //R       W
         assert(rb.size() == 4);

         assert(rb.front() == 3);
         rb.pop_front();
         //3 1 6 7 0 0 0 0
         //  R     W
         assert(rb.size() == 3);
      }
      {
         RingBuffer rb;
         //0 0 0 0
         //RW
         assert(rb.size() == 0);

         rb.push_back(1);
         //1 0 0 0
         //R W
         assert(rb.size() == 1);

         rb.push_back(2);
         //1 2 0 0
         //R   W
         assert(rb.size() == 2);

         rb.push_back(3);
         //1 2 3 0
         //R     W
         assert(rb.size() == 3);

         assert(rb.front() == 1);
         rb.pop_front();
         //1 2 3 0
         //  R   W
         assert(rb.size() == 2);

         assert(rb.front() == 2);
         rb.pop_front();
         //1 2 3 0
         //    R W
         assert(rb.size() == 1);

         rb.push_back(1);
         //1 2 3 1
         //W   R
         assert(rb.size() == 2);

         rb.push_back(6);
         //6 2 3 1
         //  W R
         assert(rb.size() == 3);

         assert(rb.front() == 3);
         rb.pop_front();
         //6 2 3 1
         //  W   R
         assert(rb.size() == 2);

         assert(rb.front() == 1);
         rb.pop_front();
         //6 2 3 1
         //R W
         assert(rb.size() == 1);

         assert(rb.front() == 6);
         rb.pop_front();
         //6 2 3 1
         //  RW
         assert(rb.size() == 0);
         assert(rb.empty() == true);
      }

   }
}