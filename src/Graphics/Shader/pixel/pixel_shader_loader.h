#pragma once
/********************************************
*  contents:   pixel shader loader
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
#include "graphics/graphics_types.h"
/******* end header inclusion *******/

namespace Core
{
   struct PixelShader;

   struct PixelShaderLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      PixelShader load(const char* buffer, uint32_t bufferSize) const;

      void unload(PixelShader& data);

   private:
      ID3D11Device* m_dev;
   };
}
