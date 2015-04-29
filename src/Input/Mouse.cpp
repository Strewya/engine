//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "input/mouse.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Mouse::Map Mouse::m_map;

   int32_t Mouse::Code(const std::string& name)
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

   std::string Mouse::Name(uint32_t code)
   {
      Map::iterator it = m_map.find(code);
      return (it != m_map.end() ? it->second : "");
   }

   void Mouse::Init()
   {
      m_map.clear();
      m_map.insert(std::make_pair(Mouse::LeftButton, "LeftButton"));
      m_map.insert(std::make_pair(Mouse::RightButton, "RightButton"));
      m_map.insert(std::make_pair(Mouse::MiddleButton, "MiddleButton"));
      m_map.insert(std::make_pair(Mouse::XButton1, "XButton1"));
      m_map.insert(std::make_pair(Mouse::XButton2, "XButton2"));
   }
}