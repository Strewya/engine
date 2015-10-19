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
   WinMsg* begin(WinMsgArray a)
   {
      return a.array;
   }
   WinMsg* end(WinMsgArray a)
   {
      return a.array + a.size;
   }


   void InputSystem::init(Memory memory)
   {
      m_staticMemory = m_dynamicMemory = memory;
      m_messages = new(m_dynamicMemory.address)WinMsg;
      m_messageCount = 0;
   }

   void InputSystem::insert(WinMsg msg)
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
}
