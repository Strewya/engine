#pragma once
/********************************************
*  contents:   vertex shader loader
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
   struct VertexShader;

   struct VertexShaderLoader
   {
   public:
      bool init(ID3D11Device* device);

      VertexShader load(InputLayout layout, void* buffer, u32 bufferSize) const;

      void unload(VertexShader& data);
      
   private:
      ID3D11Device* m_dev;
   };
}
