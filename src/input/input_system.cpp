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
      WinMsg* messages;
      u32 numMessages;
   };

   namespace input
   {
      InputSystem* init(Memory memory)
      {
         auto* input = emplace<InputSystem>(memory);
         if( !input )
         {
            CORE_LOG("Not enough memory for Input subsystem!");
            return nullptr;
         }

         input->memory = memory;
         input->numMessages = 0;
         input->messages = cast<WinMsg>(input->memory);
         
         return input;
      }

      void shutdown(InputSystem* input)
      {
         //hmmmm............
      }

      void frameUpdate(InputSystem* input, WinMsgArray* messages)
      {
         input->numMessages = 0;
         if( messages->count )
         {
            auto mem = input->memory;
            auto predicate = [input, &mem](WinMsg& msg)
            {
               auto eaten = false;
               switch( msg.type )
               {
                  case WinMsgType::GamepadAxis:
                  case WinMsgType::GamepadButton:
                  case WinMsgType::GamepadConnection:
                  case WinMsgType::KeyboardKey:
                  case WinMsgType::KeyboardText:
                  case WinMsgType::MouseButton:
                  case WinMsgType::MouseMove:
                  case WinMsgType::MouseWheel:
                  {
                     auto* m = emplace<WinMsg>(mem);
                     *m = msg;
                     ++input->numMessages;
                     eaten = true;
                  } break;
               }
               return eaten;
            };
            std::remove_if(begin(*messages), end(*messages), predicate);
            messages->count -= input->numMessages;
         }
      }

      WinMsgArray getInputEvents(InputSystem* input)
      {
         WinMsgArray result{input->messages, input->numMessages};
         return result;
      }
   }
}
