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
}
