#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <vector>
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct RingBuffer
   {
   public:
      RingBuffer(int32_t initialSize = 8, int32_t increment = 3);
      void init(int32_t initialSize, int32_t increment = 3);

      void push_back(uint32_t value);
      void pop_front();
      void clear();
      
      uint32_t front() const;
      uint32_t size() const;
      bool empty() const;

   private:
      std::vector<uint32_t> m_data;
      uint32_t m_nextWriteIndex;
      uint32_t m_nextReadIndex;
      uint32_t m_increment;
   };

   void test_ring_buffer();
}
