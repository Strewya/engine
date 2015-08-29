#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <vector>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct RingBuffer
   {
   public:
      RingBuffer(i32 initialSize = 8, i32 increment = 3);
      void init(i32 initialSize, i32 increment = 3);

      void push_back(u32 value);
      void pop_front();
      void clear();
      
      u32 front() const;
      u32 size() const;
      bool empty() const;

   private:
      std::vector<u32> m_data;
      u32 m_nextWriteIndex;
      u32 m_nextReadIndex;
      u32 m_increment;
   };

   void test_ringBuffer();
}
