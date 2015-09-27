#pragma once
/********************************************
*  contents:   pixel shader loader
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/graphics_types.h"
/******* end header inclusion *******/

namespace core
{
   struct PixelShader;

   struct PixelShaderLoader
   {
   public:
      void init(ID3D11Device* device);

      PixelShader load(void* buffer, u32 bufferSize) const;
      void unload(PixelShader& data);

   private:
      ID3D11Device* m_dev;
   };
}
