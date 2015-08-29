#pragma once
/********************************************
*  contents:   mouse
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
   struct Mouse
   {
      typedef u8 Key;
      core_class_scope Key code(std::string name);
      core_class_scope std::string name(Key code);

      enum : u8
      {
         LeftButton,
         RightButton,
         MiddleButton,
         XButton1,
         XButton2,

         KeyCount,
         Unknown = KeyCount,
      };
   };
}