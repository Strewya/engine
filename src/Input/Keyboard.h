#pragma once
/********************************************
*  contents:   keyboard key declarations
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <string>
#include <unordered_map>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Keyboard
   {
      typedef std::unordered_map<uint32_t, std::string> Map;
      static Map m_map;

   public:
      static void Init();
      static int32_t Code(const std::string& name);
      static std::string Name(uint32_t code);

      static const int32_t Unknown = -1;
      enum Key : uint8_t
      {
         Escape,
         Enter,
         Backspace,
         Shift,
         LShift,
         RShift,
         Control,
         LControl,
         RControl,
         Alt,
         LAlt,
         RAlt,
         PrintScreen,
         Pause,
         F1,
         F2,
         F3,
         F4,
         F5,
         F6,
         F7,
         F8,
         F9,
         F10,
         F11,
         F12,
         Tilde,
         _1,
         _2,
         _3,
         _4,
         _5,
         _6,
         _7,
         _8,
         _9,
         _0,
         Minus,
         Equals,
         OpenSquareBracket,
         CloseSquareBracket,
         Backslash,
         Semicolon,
         Apostrophe,
         Comma,
         Period,
         Slash,
         Tab,
         Space,
         A,
         B,
         C,
         D,
         E,
         F,
         G,
         H,
         I,
         J,
         K,
         L,
         M,
         N,
         O,
         P,
         Q,
         R,
         S,
         T,
         U,
         V,
         W,
         X,
         Y,
         Z,
         CapsLock,
         ScrollLock,
         NumLock,
         Insert,
         Home,
         PageUp,
         Delete,
         End,
         PageDown,
         ArrowUp,
         ArrowLeft,
         ArrowDown,
         ArrowRight,
         NumDivide,
         NumMultiply,
         NumSubtract,
         NumAdd,
         NumEnter,
         NumDecimal,
         Num0,
         Num1,
         Num2,
         Num3,
         Num4,
         Num5,
         Num6,
         Num7,
         Num8,
         Num9,

         KeyCount
      };
   };
}