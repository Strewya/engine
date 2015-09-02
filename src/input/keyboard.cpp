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
   Keyboard::Key Keyboard::code(const char* name)
   {
      if( strcmp(name, "Escape") == 0 ) { return Keyboard::Escape; }
      else if( strcmp(name, "Enter") == 0 ) { return Keyboard::Enter; }
      else if( strcmp(name, "Backspace") == 0 ) { return Keyboard::Backspace; }
      else if( strcmp(name, "Shift") == 0 ) { return Keyboard::Shift; }
      else if( strcmp(name, "LShift") == 0 ) { return Keyboard::LShift; }
      else if( strcmp(name, "RShift") == 0 ) { return Keyboard::RShift; }
      else if( strcmp(name, "Control") == 0 ) { return Keyboard::Control; }
      else if( strcmp(name, "LControl") == 0 ) { return Keyboard::LControl; }
      else if( strcmp(name, "RControl") == 0 ) { return Keyboard::RControl; }
      else if( strcmp(name, "Alt") == 0 ) { return Keyboard::Alt; }
      else if( strcmp(name, "LAlt") == 0 ) { return Keyboard::LAlt; }
      else if( strcmp(name, "RAlt") == 0 ) { return Keyboard::RAlt; }
      else if( strcmp(name, "PrintScreen") == 0 ) { return Keyboard::PrintScreen; }
      else if( strcmp(name, "Pause") == 0 ) { return Keyboard::Pause; }
      else if( strcmp(name, "F1") == 0 ) { return Keyboard::F1; }
      else if( strcmp(name, "F2") == 0 ) { return Keyboard::F2; }
      else if( strcmp(name, "F3") == 0 ) { return Keyboard::F3; }
      else if( strcmp(name, "F4") == 0 ) { return Keyboard::F4; }
      else if( strcmp(name, "F5") == 0 ) { return Keyboard::F5; }
      else if( strcmp(name, "F6") == 0 ) { return Keyboard::F6; }
      else if( strcmp(name, "F7") == 0 ) { return Keyboard::F7; }
      else if( strcmp(name, "F8") == 0 ) { return Keyboard::F8; }
      else if( strcmp(name, "F9") == 0 ) { return Keyboard::F9; }
      else if( strcmp(name, "F10") == 0 ) { return Keyboard::F10; }
      else if( strcmp(name, "F11") == 0 ) { return Keyboard::F11; }
      else if( strcmp(name, "F12") == 0 ) { return Keyboard::F12; }
      else if( strcmp(name, "Tilde") == 0 ) { return Keyboard::Tilde; }
      else if( strcmp(name, "_1") == 0 ) { return Keyboard::_1; }
      else if( strcmp(name, "_2") == 0 ) { return Keyboard::_2; }
      else if( strcmp(name, "_3") == 0 ) { return Keyboard::_3; }
      else if( strcmp(name, "_4") == 0 ) { return Keyboard::_4; }
      else if( strcmp(name, "_5") == 0 ) { return Keyboard::_5; }
      else if( strcmp(name, "_6") == 0 ) { return Keyboard::_6; }
      else if( strcmp(name, "_7") == 0 ) { return Keyboard::_7; }
      else if( strcmp(name, "_8") == 0 ) { return Keyboard::_8; }
      else if( strcmp(name, "_9") == 0 ) { return Keyboard::_9; }
      else if( strcmp(name, "_0") == 0 ) { return Keyboard::_0; }
      else if( strcmp(name, "Minus") == 0 ) { return Keyboard::Minus; }
      else if( strcmp(name, "Equals") == 0 ) { return Keyboard::Equals; }
      else if( strcmp(name, "OpenSquareBracket") == 0 ) { return Keyboard::OpenSquareBracket; }
      else if( strcmp(name, "CloseSquareBracket") == 0 ) { return Keyboard::CloseSquareBracket; }
      else if( strcmp(name, "Backslash") == 0 ) { return Keyboard::Backslash; }
      else if( strcmp(name, "Semicolon") == 0 ) { return Keyboard::Semicolon; }
      else if( strcmp(name, "Apostrophe") == 0 ) { return Keyboard::Apostrophe; }
      else if( strcmp(name, "Comma") == 0 ) { return Keyboard::Comma; }
      else if( strcmp(name, "Period") == 0 ) { return Keyboard::Period; }
      else if( strcmp(name, "Slash") == 0 ) { return Keyboard::Slash; }
      else if( strcmp(name, "Tab") == 0 ) { return Keyboard::Tab; }
      else if( strcmp(name, "Space") == 0 ) { return Keyboard::Space; }
      else if( strcmp(name, "A") == 0 ) { return Keyboard::A; }
      else if( strcmp(name, "B") == 0 ) { return Keyboard::B; }
      else if( strcmp(name, "C") == 0 ) { return Keyboard::C; }
      else if( strcmp(name, "D") == 0 ) { return Keyboard::D; }
      else if( strcmp(name, "E") == 0 ) { return Keyboard::E; }
      else if( strcmp(name, "F") == 0 ) { return Keyboard::F; }
      else if( strcmp(name, "G") == 0 ) { return Keyboard::G; }
      else if( strcmp(name, "H") == 0 ) { return Keyboard::H; }
      else if( strcmp(name, "I") == 0 ) { return Keyboard::I; }
      else if( strcmp(name, "J") == 0 ) { return Keyboard::J; }
      else if( strcmp(name, "K") == 0 ) { return Keyboard::K; }
      else if( strcmp(name, "L") == 0 ) { return Keyboard::L; }
      else if( strcmp(name, "M") == 0 ) { return Keyboard::M; }
      else if( strcmp(name, "N") == 0 ) { return Keyboard::N; }
      else if( strcmp(name, "O") == 0 ) { return Keyboard::O; }
      else if( strcmp(name, "P") == 0 ) { return Keyboard::P; }
      else if( strcmp(name, "Q") == 0 ) { return Keyboard::Q; }
      else if( strcmp(name, "R") == 0 ) { return Keyboard::R; }
      else if( strcmp(name, "S") == 0 ) { return Keyboard::S; }
      else if( strcmp(name, "T") == 0 ) { return Keyboard::T; }
      else if( strcmp(name, "U") == 0 ) { return Keyboard::U; }
      else if( strcmp(name, "V") == 0 ) { return Keyboard::V; }
      else if( strcmp(name, "W") == 0 ) { return Keyboard::W; }
      else if( strcmp(name, "X") == 0 ) { return Keyboard::X; }
      else if( strcmp(name, "Y") == 0 ) { return Keyboard::Y; }
      else if( strcmp(name, "Z") == 0 ) { return Keyboard::Z; }
      else if( strcmp(name, "CapsLock") == 0 ) { return Keyboard::CapsLock; }
      else if( strcmp(name, "ScrollLock") == 0 ) { return Keyboard::ScrollLock; }
      else if( strcmp(name, "NumLock") == 0 ) { return Keyboard::NumLock; }
      else if( strcmp(name, "Insert") == 0 ) { return Keyboard::Insert; }
      else if( strcmp(name, "Home") == 0 ) { return Keyboard::Home; }
      else if( strcmp(name, "PageUp") == 0 ) { return Keyboard::PageUp; }
      else if( strcmp(name, "Delete") == 0 ) { return Keyboard::Delete; }
      else if( strcmp(name, "End") == 0 ) { return Keyboard::End; }
      else if( strcmp(name, "PageDown") == 0 ) { return Keyboard::PageDown; }
      else if( strcmp(name, "ArrowUp") == 0 ) { return Keyboard::ArrowUp; }
      else if( strcmp(name, "ArrowLeft") == 0 ) { return Keyboard::ArrowLeft; }
      else if( strcmp(name, "ArrowDown") == 0 ) { return Keyboard::ArrowDown; }
      else if( strcmp(name, "ArrowRight") == 0 ) { return Keyboard::ArrowRight; }
      else if( strcmp(name, "NumDivide") == 0 ) { return Keyboard::NumDivide; }
      else if( strcmp(name, "NumMultiply") == 0 ) { return Keyboard::NumMultiply; }
      else if( strcmp(name, "NumSubtract") == 0 ) { return Keyboard::NumSubtract; }
      else if( strcmp(name, "NumAdd") == 0 ) { return Keyboard::NumAdd; }
      else if( strcmp(name, "NumEnter") == 0 ) { return Keyboard::NumEnter; }
      else if( strcmp(name, "NumDecimal") == 0 ) { return Keyboard::NumDecimal; }
      else if( strcmp(name, "Num0") == 0 ) { return Keyboard::Num0; }
      else if( strcmp(name, "Num1") == 0 ) { return Keyboard::Num1; }
      else if( strcmp(name, "Num2") == 0 ) { return Keyboard::Num2; }
      else if( strcmp(name, "Num3") == 0 ) { return Keyboard::Num3; }
      else if( strcmp(name, "Num4") == 0 ) { return Keyboard::Num4; }
      else if( strcmp(name, "Num5") == 0 ) { return Keyboard::Num5; }
      else if( strcmp(name, "Num6") == 0 ) { return Keyboard::Num6; }
      else if( strcmp(name, "Num7") == 0 ) { return Keyboard::Num7; }
      else if( strcmp(name, "Num8") == 0 ) { return Keyboard::Num8; }
      else if( strcmp(name, "Num9") == 0 ) { return Keyboard::Num9; }
      return Keyboard::Unknown;
   }

   const char* Keyboard::name(Keyboard::Key code)
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