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
   Mouse::Key Mouse::code(const char* name)
   {
      if( strcmp(name, "LeftButton") == 0 ) { return Mouse::LeftButton; }
      else if( strcmp(name, "RightButton") == 0 ) { return Mouse::RightButton; }
      else if( strcmp(name, "MiddleButton") == 0 ) { return Mouse::MiddleButton; }
      else if( strcmp(name, "XButton1") == 0 ) { return Mouse::XButton1; }
      else if( strcmp(name, "XButton2") == 0 ) { return Mouse::XButton2; }
      return Unknown;
   }

   const char* Mouse::name(Mouse::Key code)
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
