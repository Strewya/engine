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
   struct CommunicationBuffer;
   struct InputSystem;
   struct Memory;

   InputSystem* initInputSystem(Memory memory, CommunicationBuffer* receivingQueue);
   void shutdown(InputSystem* input);

   void frameUpdate(InputSystem* input, u64 timePoint);

   namespace input
   {
      
   }
}
