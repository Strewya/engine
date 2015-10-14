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
/******* end headers *******/

namespace core
{
   void InputSystem::init(Memory memory)
   {
      m_staticMemory = memory;
   }
}
