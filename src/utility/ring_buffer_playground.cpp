//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/ring_buffer.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   void test_ringBuffer()
   {
      {
         RingBuffer rb;
         CORE_ASSERT_ERR(rb.empty() == true, "Playground error");
         CORE_ASSERT_ERR(rb.size() == 0, "Playground error");

         rb.push_back(1);
         //1 0 0 0
         //R W
         CORE_ASSERT_ERR(rb.size() == 1, "Playground error");

         rb.push_back(2);
         //1 2 0 0
         //R   W
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         rb.push_back(3);
         //1 2 3 0
         //R     W
         CORE_ASSERT_ERR(rb.size() == 3, "Playground error");

         rb.push_back(4);
         //1 2 3 4 0 0 0 0
         //R       W
         CORE_ASSERT_ERR(rb.size() == 4, "Playground error");

         rb.push_back(5);
         //1 2 3 4 5 0 0 0
         //R         W
         CORE_ASSERT_ERR(rb.size() == 5, "Playground error");

         CORE_ASSERT_ERR(rb.empty() == false, "Playground error");
         rb.clear();
         CORE_ASSERT_ERR(rb.empty() == true, "Playground error");
         CORE_ASSERT_ERR(rb.size() == 0, "Playground error");
      }
      {
         RingBuffer rb;
         //0 0 0 0
         //RW
         CORE_ASSERT_ERR(rb.size() == 0, "Playground error");

         rb.push_back(1);
         //1 0 0 0
         //R W
         CORE_ASSERT_ERR(rb.size() == 1, "Playground error");

         rb.push_back(2);
         //1 2 0 0
         //R   W
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         rb.push_back(3);
         //1 2 3 0
         //R     W
         CORE_ASSERT_ERR(rb.size() == 3, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 1, "Playground error");
         rb.pop_front();
         //1 2 3 0
         //  R   W
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 2, "Playground error");
         rb.pop_front();
         //1 2 3 0
         //    R W
         CORE_ASSERT_ERR(rb.size() == 1, "Playground error");

         rb.push_back(1);
         //1 2 3 1
         //W   R
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         rb.push_back(6);
         //6 2 3 1
         //  W R
         CORE_ASSERT_ERR(rb.size() == 3, "Playground error");

         rb.push_back(7);
         //6 7 3 1
         //    RW
         //3 1 6 7 0 0 0 0
         //R       W
         CORE_ASSERT_ERR(rb.size() == 4, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 3, "Playground error");
         rb.pop_front();
         //3 1 6 7 0 0 0 0
         //  R     W
         CORE_ASSERT_ERR(rb.size() == 3, "Playground error");
      }
      {
         RingBuffer rb;
         //0 0 0 0
         //RW
         CORE_ASSERT_ERR(rb.size() == 0, "Playground error");

         rb.push_back(1);
         //1 0 0 0
         //R W
         CORE_ASSERT_ERR(rb.size() == 1, "Playground error");

         rb.push_back(2);
         //1 2 0 0
         //R   W
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         rb.push_back(3);
         //1 2 3 0
         //R     W
         CORE_ASSERT_ERR(rb.size() == 3, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 1, "Playground error");
         rb.pop_front();
         //1 2 3 0
         //  R   W
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 2, "Playground error");
         rb.pop_front();
         //1 2 3 0
         //    R W
         CORE_ASSERT_ERR(rb.size() == 1, "Playground error");

         rb.push_back(1);
         //1 2 3 1
         //W   R
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         rb.push_back(6);
         //6 2 3 1
         //  W R
         CORE_ASSERT_ERR(rb.size() == 3, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 3, "Playground error");
         rb.pop_front();
         //6 2 3 1
         //  W   R
         CORE_ASSERT_ERR(rb.size() == 2, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 1, "Playground error");
         rb.pop_front();
         //6 2 3 1
         //R W
         CORE_ASSERT_ERR(rb.size() == 1, "Playground error");

         CORE_ASSERT_ERR(rb.front() == 6, "Playground error");
         rb.pop_front();
         //6 2 3 1
         //  RW
         CORE_ASSERT_ERR(rb.size() == 0, "Playground error");
         CORE_ASSERT_ERR(rb.empty() == true, "Playground error");
      }

   }
}