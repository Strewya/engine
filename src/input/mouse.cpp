//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "input/mouse.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Mouse::Key Mouse::code(std::string name)
   {
      if(name == "LeftButton") { return Mouse::LeftButton; }
      else if(name == "RightButton") { return Mouse::RightButton; }
      else if(name == "MiddleButton") { return Mouse::MiddleButton; }
      else if(name == "XButton1") { return Mouse::XButton1; }
      else if(name == "XButton2") { return Mouse::XButton2; }
      return Unknown;
   }

   std::string Mouse::name(Mouse::Key code)
   {
      switch( code )
      {
         case Mouse::LeftButton: { return "LeftButton"; } break;
         case Mouse::RightButton: { return "RightButton"; } break;
         case Mouse::MiddleButton: { return "MiddleButton"; } break;
         case Mouse::XButton1: { return "XButton1"; } break;
         case Mouse::XButton2: { return "XButton2"; } break;
      }
      return "";
   }
}
