//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "input/input_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "input/gamepad.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "window/window_message.h"
/******* end headers *******/

namespace core
{
   struct WinMsg;

   struct WinMsgArray
   {
      WinMsg* array;
      u32 size;
   };

   struct InputSystem
   {
      Memory memory;
      Memory frameMemory;
      WinMsg* messages;
      u32 numMessage;
   };

   WinMsg* begin(WinMsgArray a)
   {
      return a.array;
   }
   WinMsg* end(WinMsgArray a)
   {
      return a.array + a.size;
   }

   InputSystem* initInputSystem(Memory memory, CommunicationBuffer* receivingQueue)
   {
      auto* input = emplace<InputSystem>(memory);
      if( !input )
      {
         CORE_LOG("Not enough memory for Input subsystem!");
         return nullptr;
      }

      input->memory = input->frameMemory = memory;
      input->numMessage = 0;
      input->messages = cast<WinMsg>(input->frameMemory);
      if( !input->messages )
      {
         CORE_LOG("Not enough memory for the message array!");
         return nullptr;
      }

      return input;
   }

   void shutdown(InputSystem* input)
   {
      //hmmmm............
   }

   void frameUpdate(InputSystem* input, u64 timePoint)
   {

   }
#if 0
   void insert(WinMsg msg)
   {
      WinMsg* msgSlot = emplace<WinMsg>(m_dynamicMemory);
      *msgSlot = msg;
      ++m_messageCount;
   }

   void InputSystem::clear()
   {
      m_messageCount = 0;
   }

   WinMsgArray InputSystem::getMessageArray() const
   {
      WinMsgArray result{m_messages, m_messageCount};
      return result;
   }
endif
}
