#pragma once
/********************************************
*  contents:   input system
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct WinMsg;

   struct InputSystem
   {
   public:
      void init(Memory memory);

      void insert(WinMsg msg);
      

   private:
      Memory m_staticMemory;
      Memory m_dynamicMemory;
      WinMsg* m_messages;
      u32 m_messageCount;
   };
}
