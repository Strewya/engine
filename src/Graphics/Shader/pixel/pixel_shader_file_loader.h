#pragma once
/********************************************
*  contents:   file loader for pixel shaders
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/shader/pixel/pixel_shader_loader.h"
/******* end header inclusion *******/

namespace Core
{
   class PixelShader;

   class PixelShaderFileLoader
   {
   public:
      bool init(PixelShaderLoader& loader);
      bool shutdown();

      PixelShader load(const char* filename) const;

      void unload(PixelShader& data);

   private:
      PixelShaderLoader* m_loader;
   };
}
