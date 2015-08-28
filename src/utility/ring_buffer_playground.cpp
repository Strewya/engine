//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/ring_buffer.h"
/******* c++ headers *******/
#include <cassert>
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   void test_ringBuffer()
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