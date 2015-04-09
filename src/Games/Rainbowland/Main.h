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
      Window* window;

      GraphicsSystem graphicsSystem;

      HTexture textureAtlasHandle;
      HVertexShader mainVSHandle;
      HPixelShader mainPSHandle;
      HPixelShader healthPSHandle;

      bool init(Window& window);
      bool shutdown();

      bool tickLogic(uint64_t updateTime);
      void tickRender(uint64_t updateTime);
   };
}
