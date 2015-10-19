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

   struct WinMsgArray
   {
      WinMsg* array;
      u32 size;
   };

   WinMsg* begin(WinMsgArray a);
   WinMsg* end(WinMsgArray a);

   struct InputSystem
   {
   public:
      void init(Memory memory);

      void insert(WinMsg msg);
      void clear();

      WinMsgArray getMessageArray() const;
      
   private:
      Memory m_staticMemory;
      Memory m_dynamicMemory;
      WinMsg* m_messages;
      u32 m_messageCount;
   };
}
