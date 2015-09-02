//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "input/gamepad.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Gamepad::Key Gamepad::code(const char* name)
   {
      if( strcmp(name, "DPadUp") == 0 ) { return Gamepad::DPadUp; }
      else if( strcmp(name, "DPadDown") == 0 ) { return Gamepad::DPadDown; }
      else if( strcmp(name, "DPadLeft") == 0 ) { return Gamepad::DPadLeft; }
      else if( strcmp(name, "DPadRight") == 0 ) { return Gamepad::DPadRight; }
      else if( strcmp(name, "LeftButton") == 0 ) { return Gamepad::LeftButton; }
      else if( strcmp(name, "TopButton") == 0 ) { return Gamepad::TopButton; }
      else if( strcmp(name, "RightButton") == 0 ) { return Gamepad::RightButton; }
      else if( strcmp(name, "BottomButton") == 0 ) { return Gamepad::BottomButton; }
      else if( strcmp(name, "LeftThumb") == 0 ) { return Gamepad::LeftThumb; }
      else if( strcmp(name, "RightThumb") == 0 ) { return Gamepad::RightThumb; }
      else if( strcmp(name, "LeftShoulder") == 0 ) { return Gamepad::LeftShoulder; }
      else if( strcmp(name, "RightShoulder") == 0 ) { return Gamepad::RightShoulder; }
      else if( strcmp(name, "Start") == 0 ) { return Gamepad::Start; }
      else if( strcmp(name, "Back") == 0 ) { return Gamepad::Back; }
      else if( strcmp(name, "LeftStick") == 0 ) { return Gamepad::LeftStick; }
      else if( strcmp(name, "RightStick") == 0 ) { return Gamepad::RightStick; }
      else if( strcmp(name, "LeftTrigger") == 0 ) { return Gamepad::LeftTrigger; }
      else if( strcmp(name, "RightTrigger") == 0 ) { return Gamepad::RightTrigger; }
      return Gamepad::Unknown;
   }

   const char* Gamepad::name(Gamepad::Key code)
   {
      switch( code )
      {
         case Gamepad::DPadUp: { return "DPadUp"; } break;
         case Gamepad::DPadDown: { return "DPadDown"; } break;
         case Gamepad::DPadLeft: { return "DPadLeft"; } break;
         case Gamepad::DPadRight: { return "DPadRight"; } break;
         case Gamepad::LeftButton: { return "LeftButton"; } break;
         case Gamepad::TopButton: { return "TopButton"; } break;
         case Gamepad::RightButton: { return "RightButton"; } break;
         case Gamepad::BottomButton: { return "BottomButton"; } break;
         case Gamepad::LeftThumb: { return "LeftThumb"; } break;
         case Gamepad::RightThumb: { return "RightThumb"; } break;
         case Gamepad::LeftShoulder: { return "LeftShoulder"; } break;
         case Gamepad::RightShoulder: { return "RightShoulder"; } break;
         case Gamepad::Start: { return "Start"; } break;
         case Gamepad::Back: { return "Back"; } break;
         case Gamepad::LeftStick: { return "LeftStick"; } break;
         case Gamepad::RightStick: { return "RightStick"; } break;
         case Gamepad::LeftTrigger: { return "LeftTrigger"; } break;
         case Gamepad::RightTrigger: { return "RightTrigger"; } break;
      }
      return "";
   }
}