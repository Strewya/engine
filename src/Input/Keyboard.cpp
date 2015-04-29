//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "input/keyboard.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Keyboard::Map Keyboard::m_map;

   int32_t Keyboard::Code(const std::string& name)
   {
      Map::iterator it = m_map.begin();
      Map::iterator end = m_map.end();
      for( auto& p : m_map )
      {
         if( p.second == name )
            return p.first;
      }
      return Unknown;
   }

   std::string Keyboard::Name(uint32_t code)
   {
      Map::iterator it = m_map.find(code);
      return (it != m_map.end() ? it->second : "");
   }

   void Keyboard::Init()
   {
      m_map.insert(std::make_pair(Escape, "Escape"));
      m_map.insert(std::make_pair(Enter, "Enter"));
      m_map.insert(std::make_pair(Backspace, "Backspace"));
      m_map.insert(std::make_pair(Shift, "Shift"));
      m_map.insert(std::make_pair(LShift, "LShift"));
      m_map.insert(std::make_pair(RShift, "RShift"));
      m_map.insert(std::make_pair(Control, "Control"));
      m_map.insert(std::make_pair(LControl, "LControl"));
      m_map.insert(std::make_pair(RControl, "RControl"));
      m_map.insert(std::make_pair(Alt, "Alt"));
      m_map.insert(std::make_pair(LAlt, "LAlt"));
      m_map.insert(std::make_pair(RAlt, "RAlt"));
      m_map.insert(std::make_pair(PrintScreen, "PrintScreen"));
      m_map.insert(std::make_pair(Pause, "Pause"));
      m_map.insert(std::make_pair(F1, "F1"));
      m_map.insert(std::make_pair(F2, "F2"));
      m_map.insert(std::make_pair(F3, "F3"));
      m_map.insert(std::make_pair(F4, "F4"));
      m_map.insert(std::make_pair(F5, "F5"));
      m_map.insert(std::make_pair(F6, "F6"));
      m_map.insert(std::make_pair(F7, "F7"));
      m_map.insert(std::make_pair(F8, "F8"));
      m_map.insert(std::make_pair(F9, "F9"));
      m_map.insert(std::make_pair(F10, "F10"));
      m_map.insert(std::make_pair(F11, "F11"));
      m_map.insert(std::make_pair(F12, "F12"));
      m_map.insert(std::make_pair(Tilde, "Tilde"));
      m_map.insert(std::make_pair(_1, "1"));
      m_map.insert(std::make_pair(_2, "2"));
      m_map.insert(std::make_pair(_3, "3"));
      m_map.insert(std::make_pair(_4, "4"));
      m_map.insert(std::make_pair(_5, "5"));
      m_map.insert(std::make_pair(_6, "6"));
      m_map.insert(std::make_pair(_7, "7"));
      m_map.insert(std::make_pair(_8, "8"));
      m_map.insert(std::make_pair(_9, "9"));
      m_map.insert(std::make_pair(_0, "0"));
      m_map.insert(std::make_pair(Minus, "Minus"));
      m_map.insert(std::make_pair(Equals, "Equals"));
      m_map.insert(std::make_pair(OpenSquareBracket, "OpenSquareBracket"));
      m_map.insert(std::make_pair(CloseSquareBracket, "CloseSquareBracket"));
      m_map.insert(std::make_pair(Backslash, "Backslash"));
      m_map.insert(std::make_pair(Semicolon, "Semicolon"));
      m_map.insert(std::make_pair(Apostrophe, "Apostrophe"));
      m_map.insert(std::make_pair(Comma, "Comma"));
      m_map.insert(std::make_pair(Period, "Period"));
      m_map.insert(std::make_pair(Slash, "Slash"));
      m_map.insert(std::make_pair(Tab, "Tab"));
      m_map.insert(std::make_pair(Space, "Space"));
      m_map.insert(std::make_pair(A, "A"));
      m_map.insert(std::make_pair(B, "B"));
      m_map.insert(std::make_pair(C, "C"));
      m_map.insert(std::make_pair(D, "D"));
      m_map.insert(std::make_pair(E, "E"));
      m_map.insert(std::make_pair(F, "F"));
      m_map.insert(std::make_pair(G, "G"));
      m_map.insert(std::make_pair(H, "H"));
      m_map.insert(std::make_pair(I, "I"));
      m_map.insert(std::make_pair(J, "J"));
      m_map.insert(std::make_pair(K, "K"));
      m_map.insert(std::make_pair(L, "L"));
      m_map.insert(std::make_pair(M, "M"));
      m_map.insert(std::make_pair(N, "N"));
      m_map.insert(std::make_pair(O, "O"));
      m_map.insert(std::make_pair(P, "P"));
      m_map.insert(std::make_pair(Q, "Q"));
      m_map.insert(std::make_pair(R, "R"));
      m_map.insert(std::make_pair(S, "S"));
      m_map.insert(std::make_pair(T, "T"));
      m_map.insert(std::make_pair(U, "U"));
      m_map.insert(std::make_pair(V, "V"));
      m_map.insert(std::make_pair(W, "W"));
      m_map.insert(std::make_pair(X, "X"));
      m_map.insert(std::make_pair(Y, "Y"));
      m_map.insert(std::make_pair(Z, "Z"));
      m_map.insert(std::make_pair(CapsLock, "CapsLock"));
      m_map.insert(std::make_pair(ScrollLock, "ScrollLock"));
      m_map.insert(std::make_pair(NumLock, "NumLock"));
      m_map.insert(std::make_pair(Insert, "Insert"));
      m_map.insert(std::make_pair(Home, "Home"));
      m_map.insert(std::make_pair(PageUp, "PageUp"));
      m_map.insert(std::make_pair(Delete, "Delete"));
      m_map.insert(std::make_pair(End, "End"));
      m_map.insert(std::make_pair(PageDown, "PageDown"));
      m_map.insert(std::make_pair(ArrowUp, "ArrowUp"));
      m_map.insert(std::make_pair(ArrowLeft, "ArrowLeft"));
      m_map.insert(std::make_pair(ArrowDown, "ArrowDown"));
      m_map.insert(std::make_pair(ArrowRight, "ArrowRight"));
      m_map.insert(std::make_pair(NumDivide, "NumDivide"));
      m_map.insert(std::make_pair(NumMultiply, "NumMultiply"));
      m_map.insert(std::make_pair(NumSubtract, "NumSubtract"));
      m_map.insert(std::make_pair(NumAdd, "NumAdd"));
      m_map.insert(std::make_pair(NumEnter, "NumEnter"));
      m_map.insert(std::make_pair(NumDecimal, "NumDecimal"));
      m_map.insert(std::make_pair(Num0, "Num0"));
      m_map.insert(std::make_pair(Num1, "Num1"));
      m_map.insert(std::make_pair(Num2, "Num2"));
      m_map.insert(std::make_pair(Num3, "Num3"));
      m_map.insert(std::make_pair(Num4, "Num4"));
      m_map.insert(std::make_pair(Num5, "Num5"));
      m_map.insert(std::make_pair(Num6, "Num6"));
      m_map.insert(std::make_pair(Num7, "Num7"));
      m_map.insert(std::make_pair(Num8, "Num8"));
      m_map.insert(std::make_pair(Num9, "Num9"));
   }
}