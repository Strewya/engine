#pragma once
/********************************************
*  contents:   gamepad key declarations
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Gamepad
   {
      typedef u8 Key;

      core_class_scope Key code(std::string name);
      core_class_scope std::string name(Key code);

      enum : u8
      {
         DPadUp,
         DPadDown,
         DPadLeft,
         DPadRight,
         LeftButton,
         TopButton,
         RightButton,
         BottomButton,
         LeftThumb,
         RightThumb,
         LeftShoulder,
         RightShoulder,
         Start,
         Back,
         LeftStick,
         RightStick,
         LeftTrigger,
         RightTrigger,

         KeyCount,
         Unknown = KeyCount,
      };
   };
}