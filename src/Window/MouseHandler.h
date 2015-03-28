#pragma once
/********************************************
*  contents:   MouseHandler
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include <Window/myWindows.h>
/******* extra headers *******/
#include <Window/WindowEvent.h>
/******* end header inclusion *******/

namespace Core
{
   class MouseHandler
   {
   public:
      MouseHandler();

      void setRelativeMouseMove(bool isRelative, uint32_t centerX, uint32_t centerY);


      bool handle(WindowEvent& we, uint32_t msg, WPARAM wp, LPARAM lp);

   private:
      uint32_t m_centerX;
      uint32_t m_centerY;
      bool m_relativeMouseMove;
   };
}
