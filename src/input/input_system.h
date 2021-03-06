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
#include "utility/array.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;
   struct InputSystem;
   struct Memory;
   struct WinMsg;
   typedef Array<WinMsg> WinMsgArray;

   namespace input
   {
      InputSystem* init(Memory memory);
      void shutdown(InputSystem* input);

      void frameUpdate(InputSystem* input, WinMsgArray* messages);

      WinMsgArray getInputEvents(InputSystem* input);
   }
}
