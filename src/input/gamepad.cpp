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
   Gamepad::Key Gamepad::code(std::string name)
   {
      if( name == "DPadUp" ) { return Gamepad::DPadUp; }
      else if( name == "DPadDown" ) { return Gamepad::DPadDown; }
      else if( name == "DPadLeft" ) { return Gamepad::DPadLeft; }
      else if( name == "DPadRight" ) { return Gamepad::DPadRight; }
      else if( name == "LeftButton" ) { return Gamepad::LeftButton; }
      else if( name == "TopButton" ) { return Gamepad::TopButton; }
      else if( name == "RightButton" ) { return Gamepad::RightButton; }
      else if( name == "BottomButton" ) { return Gamepad::BottomButton; }
      else if( name == "LeftThumb" ) { return Gamepad::LeftThumb; }
      else if( name == "RightThumb" ) { return Gamepad::RightThumb; }
      else if( name == "LeftShoulder" ) { return Gamepad::LeftShoulder; }
      else if( name == "RightShoulder" ) { return Gamepad::RightShoulder; }
      else if( name == "Start" ) { return Gamepad::Start; }
      else if( name == "Back" ) { return Gamepad::Back; }
      else if( name == "LeftStick" ) { return Gamepad::LeftStick; }
      else if( name == "RightStick" ) { return Gamepad::RightStick; }
      else if( name == "LeftTrigger" ) { return Gamepad::LeftTrigger; }
      else if( name == "RightTrigger" ) { return Gamepad::RightTrigger; }
      return Gamepad::Unknown;
   }

   std::string Gamepad::name(Gamepad::Key code)
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