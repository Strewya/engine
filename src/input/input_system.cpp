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
   }


}
