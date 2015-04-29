#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
#include <vector>
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

      PixelShader load(const std::string& filename) const;

      void unload(PixelShader& data);

   private:
      PixelShaderLoader* m_loader;
   };
}
