//headers should be ordered alphabetically!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <input/gamepad.h>
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Gamepad::Map Gamepad::m_map;

   int32_t Gamepad::Code(std::string name)
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

   std::string Gamepad::Name(uint32_t code)
   {
      Map::iterator it = m_map.find(code);
      return (it != m_map.end() ? it->second : "");
   }

   void Gamepad::Init()
   {
      m_map.clear();
      m_map.emplace(Gamepad::DPadUp, "DPadUp");
      m_map.emplace(Gamepad::DPadDown, "DPadDown");
      m_map.emplace(Gamepad::DPadLeft, "DPadLeft");
      m_map.emplace(Gamepad::DPadRight, "DPadRight");
      m_map.emplace(Gamepad::LeftButton, "LeftButton");
      m_map.emplace(Gamepad::TopButton, "TopButton");
      m_map.emplace(Gamepad::RightButton, "RightButton");
      m_map.emplace(Gamepad::BottomButton, "BottomButton");
      m_map.emplace(Gamepad::LeftThumb, "LeftThumb");
      m_map.emplace(Gamepad::RightThumb, "RightThumb");
      m_map.emplace(Gamepad::LeftShoulder, "LeftShoulder");
      m_map.emplace(Gamepad::RightShoulder, "RightShoulder");
      m_map.emplace(Gamepad::Start, "Start");
      m_map.emplace(Gamepad::Back, "Back");
      m_map.emplace(Gamepad::LeftStick, "LeftStick");
      m_map.emplace(Gamepad::RightStick, "RightStick");
      m_map.emplace(Gamepad::LeftTrigger, "LeftTrigger");
      m_map.emplace(Gamepad::RightTrigger, "RightTrigger");
   }
}