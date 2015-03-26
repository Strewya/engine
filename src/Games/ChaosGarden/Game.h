#pragma once
/********************************************
*  class:   Game
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Window;

   class Game
   {
   public:
      Window* m_window;



      bool init(Window& window);
      void shutdown();

      bool tickLogic(uint64_t updateTime);
      void tickRender(uint64_t updateTime);
   };
}
