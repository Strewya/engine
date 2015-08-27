//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "input/keyboard.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Keyboard::Key Keyboard::code(std::string name)
   {
      if( name == "Escape" ) { return Keyboard::Escape; }
      else if( name == "Enter" ) { return Keyboard::Enter; }
      else if( name == "Backspace" ) { return Keyboard::Backspace; }
      else if( name == "Shift" ) { return Keyboard::Shift; }
      else if( name == "LShift" ) { return Keyboard::LShift; }
      else if( name == "RShift" ) { return Keyboard::RShift; }
      else if( name == "Control" ) { return Keyboard::Control; }
      else if( name == "LControl" ) { return Keyboard::LControl; }
      else if( name == "RControl" ) { return Keyboard::RControl; }
      else if( name == "Alt" ) { return Keyboard::Alt; }
      else if( name == "LAlt" ) { return Keyboard::LAlt; }
      else if( name == "RAlt" ) { return Keyboard::RAlt; }
      else if( name == "PrintScreen" ) { return Keyboard::PrintScreen; }
      else if( name == "Pause" ) { return Keyboard::Pause; }
      else if( name == "F1" ) { return Keyboard::F1; }
      else if( name == "F2" ) { return Keyboard::F2; }
      else if( name == "F3" ) { return Keyboard::F3; }
      else if( name == "F4" ) { return Keyboard::F4; }
      else if( name == "F5" ) { return Keyboard::F5; }
      else if( name == "F6" ) { return Keyboard::F6; }
      else if( name == "F7" ) { return Keyboard::F7; }
      else if( name == "F8" ) { return Keyboard::F8; }
      else if( name == "F9" ) { return Keyboard::F9; }
      else if( name == "F10" ) { return Keyboard::F10; }
      else if( name == "F11" ) { return Keyboard::F11; }
      else if( name == "F12" ) { return Keyboard::F12; }
      else if( name == "Tilde" ) { return Keyboard::Tilde; }
      else if( name == "_1" ) { return Keyboard::_1; }
      else if( name == "_2" ) { return Keyboard::_2; }
      else if( name == "_3" ) { return Keyboard::_3; }
      else if( name == "_4" ) { return Keyboard::_4; }
      else if( name == "_5" ) { return Keyboard::_5; }
      else if( name == "_6" ) { return Keyboard::_6; }
      else if( name == "_7" ) { return Keyboard::_7; }
      else if( name == "_8" ) { return Keyboard::_8; }
      else if( name == "_9" ) { return Keyboard::_9; }
      else if( name == "_0" ) { return Keyboard::_0; }
      else if( name == "Minus" ) { return Keyboard::Minus; }
      else if( name == "Equals" ) { return Keyboard::Equals; }
      else if( name == "OpenSquareBracket" ) { return Keyboard::OpenSquareBracket; }
      else if( name == "CloseSquareBracket" ) { return Keyboard::CloseSquareBracket; }
      else if( name == "Backslash" ) { return Keyboard::Backslash; }
      else if( name == "Semicolon" ) { return Keyboard::Semicolon; }
      else if( name == "Apostrophe" ) { return Keyboard::Apostrophe; }
      else if( name == "Comma" ) { return Keyboard::Comma; }
      else if( name == "Period" ) { return Keyboard::Period; }
      else if( name == "Slash" ) { return Keyboard::Slash; }
      else if( name == "Tab" ) { return Keyboard::Tab; }
      else if( name == "Space" ) { return Keyboard::Space; }
      else if( name == "A" ) { return Keyboard::A; }
      else if( name == "B" ) { return Keyboard::B; }
      else if( name == "C" ) { return Keyboard::C; }
      else if( name == "D" ) { return Keyboard::D; }
      else if( name == "E" ) { return Keyboard::E; }
      else if( name == "F" ) { return Keyboard::F; }
      else if( name == "G" ) { return Keyboard::G; }
      else if( name == "H" ) { return Keyboard::H; }
      else if( name == "I" ) { return Keyboard::I; }
      else if( name == "J" ) { return Keyboard::J; }
      else if( name == "K" ) { return Keyboard::K; }
      else if( name == "L" ) { return Keyboard::L; }
      else if( name == "M" ) { return Keyboard::M; }
      else if( name == "N" ) { return Keyboard::N; }
      else if( name == "O" ) { return Keyboard::O; }
      else if( name == "P" ) { return Keyboard::P; }
      else if( name == "Q" ) { return Keyboard::Q; }
      else if( name == "R" ) { return Keyboard::R; }
      else if( name == "S" ) { return Keyboard::S; }
      else if( name == "T" ) { return Keyboard::T; }
      else if( name == "U" ) { return Keyboard::U; }
      else if( name == "V" ) { return Keyboard::V; }
      else if( name == "W" ) { return Keyboard::W; }
      else if( name == "X" ) { return Keyboard::X; }
      else if( name == "Y" ) { return Keyboard::Y; }
      else if( name == "Z" ) { return Keyboard::Z; }
      else if( name == "CapsLock" ) { return Keyboard::CapsLock; }
      else if( name == "ScrollLock" ) { return Keyboard::ScrollLock; }
      else if( name == "NumLock" ) { return Keyboard::NumLock; }
      else if( name == "Insert" ) { return Keyboard::Insert; }
      else if( name == "Home" ) { return Keyboard::Home; }
      else if( name == "PageUp" ) { return Keyboard::PageUp; }
      else if( name == "Delete" ) { return Keyboard::Delete; }
      else if( name == "End" ) { return Keyboard::End; }
      else if( name == "PageDown" ) { return Keyboard::PageDown; }
      else if( name == "ArrowUp" ) { return Keyboard::ArrowUp; }
      else if( name == "ArrowLeft" ) { return Keyboard::ArrowLeft; }
      else if( name == "ArrowDown" ) { return Keyboard::ArrowDown; }
      else if( name == "ArrowRight" ) { return Keyboard::ArrowRight; }
      else if( name == "NumDivide" ) { return Keyboard::NumDivide; }
      else if( name == "NumMultiply" ) { return Keyboard::NumMultiply; }
      else if( name == "NumSubtract" ) { return Keyboard::NumSubtract; }
      else if( name == "NumAdd" ) { return Keyboard::NumAdd; }
      else if( name == "NumEnter" ) { return Keyboard::NumEnter; }
      else if( name == "NumDecimal" ) { return Keyboard::NumDecimal; }
      else if( name == "Num0" ) { return Keyboard::Num0; }
      else if( name == "Num1" ) { return Keyboard::Num1; }
      else if( name == "Num2" ) { return Keyboard::Num2; }
      else if( name == "Num3" ) { return Keyboard::Num3; }
      else if( name == "Num4" ) { return Keyboard::Num4; }
      else if( name == "Num5" ) { return Keyboard::Num5; }
      else if( name == "Num6" ) { return Keyboard::Num6; }
      else if( name == "Num7" ) { return Keyboard::Num7; }
      else if( name == "Num8" ) { return Keyboard::Num8; }
      else if( name == "Num9" ) { return Keyboard::Num9; }
      return Keyboard::Unknown;
   }

   std::string Keyboard::name(Keyboard::Key code)
   {
      switch( code )
      {
         case Keyboard::Escape: { return "Escape"; } break;
         case Keyboard::Enter: { return "Enter"; } break;
         case Keyboard::Backspace: { return "Backspace"; } break;
         case Keyboard::Shift: { return "Shift"; } break;
         case Keyboard::LShift: { return "LShift"; } break;
         case Keyboard::RShift: { return "RShift"; } break;
         case Keyboard::Control: { return "Control"; } break;
         case Keyboard::LControl: { return "LControl"; } break;
         case Keyboard::RControl: { return "RControl"; } break;
         case Keyboard::Alt: { return "Alt"; } break;
         case Keyboard::LAlt: { return "LAlt"; } break;
         case Keyboard::RAlt: { return "RAlt"; } break;
         case Keyboard::PrintScreen: { return "PrintScreen"; } break;
         case Keyboard::Pause: { return "Pause"; } break;
         case Keyboard::F1: { return "F1"; } break;
         case Keyboard::F2: { return "F2"; } break;
         case Keyboard::F3: { return "F3"; } break;
         case Keyboard::F4: { return "F4"; } break;
         case Keyboard::F5: { return "F5"; } break;
         case Keyboard::F6: { return "F6"; } break;
         case Keyboard::F7: { return "F7"; } break;
         case Keyboard::F8: { return "F8"; } break;
         case Keyboard::F9: { return "F9"; } break;
         case Keyboard::F10: { return "F10"; } break;
         case Keyboard::F11: { return "F11"; } break;
         case Keyboard::F12: { return "F12"; } break;
         case Keyboard::Tilde: { return "Tilde"; } break;
         case Keyboard::_1: { return "_1"; } break;
         case Keyboard::_2: { return "_2"; } break;
         case Keyboard::_3: { return "_3"; } break;
         case Keyboard::_4: { return "_4"; } break;
         case Keyboard::_5: { return "_5"; } break;
         case Keyboard::_6: { return "_6"; } break;
         case Keyboard::_7: { return "_7"; } break;
         case Keyboard::_8: { return "_8"; } break;
         case Keyboard::_9: { return "_9"; } break;
         case Keyboard::_0: { return "_0"; } break;
         case Keyboard::Minus: { return "Minus"; } break;
         case Keyboard::Equals: { return "Equals"; } break;
         case Keyboard::OpenSquareBracket: { return "OpenSquareBracket"; } break;
         case Keyboard::CloseSquareBracket: { return "CloseSquareBracket"; } break;
         case Keyboard::Backslash: { return "Backslash"; } break;
         case Keyboard::Semicolon: { return "Semicolon"; } break;
         case Keyboard::Apostrophe: { return "Apostrophe"; } break;
         case Keyboard::Comma: { return "Comma"; } break;
         case Keyboard::Period: { return "Period"; } break;
         case Keyboard::Slash: { return "Slash"; } break;
         case Keyboard::Tab: { return "Tab"; } break;
         case Keyboard::Space: { return "Space"; } break;
         case Keyboard::A: { return "A"; } break;
         case Keyboard::B: { return "B"; } break;
         case Keyboard::C: { return "C"; } break;
         case Keyboard::D: { return "D"; } break;
         case Keyboard::E: { return "E"; } break;
         case Keyboard::F: { return "F"; } break;
         case Keyboard::G: { return "G"; } break;
         case Keyboard::H: { return "H"; } break;
         case Keyboard::I: { return "I"; } break;
         case Keyboard::J: { return "J"; } break;
         case Keyboard::K: { return "K"; } break;
         case Keyboard::L: { return "L"; } break;
         case Keyboard::M: { return "M"; } break;
         case Keyboard::N: { return "N"; } break;
         case Keyboard::O: { return "O"; } break;
         case Keyboard::P: { return "P"; } break;
         case Keyboard::Q: { return "Q"; } break;
         case Keyboard::R: { return "R"; } break;
         case Keyboard::S: { return "S"; } break;
         case Keyboard::T: { return "T"; } break;
         case Keyboard::U: { return "U"; } break;
         case Keyboard::V: { return "V"; } break;
         case Keyboard::W: { return "W"; } break;
         case Keyboard::X: { return "X"; } break;
         case Keyboard::Y: { return "Y"; } break;
         case Keyboard::Z: { return "Z"; } break;
         case Keyboard::CapsLock: { return "CapsLock"; } break;
         case Keyboard::ScrollLock: { return "ScrollLock"; } break;
         case Keyboard::NumLock: { return "NumLock"; } break;
         case Keyboard::Insert: { return "Insert"; } break;
         case Keyboard::Home: { return "Home"; } break;
         case Keyboard::PageUp: { return "PageUp"; } break;
         case Keyboard::Delete: { return "Delete"; } break;
         case Keyboard::End: { return "End"; } break;
         case Keyboard::PageDown: { return "PageDown"; } break;
         case Keyboard::ArrowUp: { return "ArrowUp"; } break;
         case Keyboard::ArrowLeft: { return "ArrowLeft"; } break;
         case Keyboard::ArrowDown: { return "ArrowDown"; } break;
         case Keyboard::ArrowRight: { return "ArrowRight"; } break;
         case Keyboard::NumDivide: { return "NumDivide"; } break;
         case Keyboard::NumMultiply: { return "NumMultiply"; } break;
         case Keyboard::NumSubtract: { return "NumSubtract"; } break;
         case Keyboard::NumAdd: { return "NumAdd"; } break;
         case Keyboard::NumEnter: { return "NumEnter"; } break;
         case Keyboard::NumDecimal: { return "NumDecimal"; } break;
         case Keyboard::Num0: { return "Num0"; } break;
         case Keyboard::Num1: { return "Num1"; } break;
         case Keyboard::Num2: { return "Num2"; } break;
         case Keyboard::Num3: { return "Num3"; } break;
         case Keyboard::Num4: { return "Num4"; } break;
         case Keyboard::Num5: { return "Num5"; } break;
         case Keyboard::Num6: { return "Num6"; } break;
         case Keyboard::Num7: { return "Num7"; } break;
         case Keyboard::Num8: { return "Num8"; } break;
         case Keyboard::Num9: { return "Num9"; } break;
      }
      return "";
   }

}