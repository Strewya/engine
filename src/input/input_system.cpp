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
#include "utility/communication_buffer.h"
#include "window/window_message.h"
/******* end headers *******/

namespace core
{
   struct InputSystem
   {
      Memory memory;
      CommunicationBuffer* receivingQueue;
      WinMsg* messages;
      u32 numMessages;
   };

   namespace input
   {
      InputSystem* init(Memory memory, CommunicationBuffer* receivingQueue)
      {
         auto* input = emplace<InputSystem>(memory);
         if( !input )
         {
            CORE_LOG("Not enough memory for Input subsystem!");
            return nullptr;
         }

         input->memory = memory;
         input->receivingQueue = receivingQueue;
         input->numMessages = 0;
         input->messages = cast<WinMsg>(input->memory);
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
         auto messages = input->receivingQueue->getMessagesUntil(timePoint);
         auto mem = input->memory;
         input->messages = emplaceArray<WinMsg>(mem, messages.count);
         std::copy(begin(messages), end(messages), input->messages);

      }

      WinMsgArray getInputDeviceEvents(InputSystem* input)
      {
         WinMsgArray result{input->messages, input->numMessages};
         return result;
      }

   }
}
