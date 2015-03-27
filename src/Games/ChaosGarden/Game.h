#pragma once
/********************************************
*  class:   Game
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
/******* end header inclusion *******/

namespace Core
{
   class Window;

   class Game
   {
   public:
      Window* m_window;

      GraphicsSystem m_graphicsSystem;
      TextureStore m_textureCache;
      FontStore m_fontCache;

      bool init(Window& window);
      void shutdown();

      bool tickLogic(uint64_t updateTime);
      void tickRender(uint64_t updateTime);
   };
}
