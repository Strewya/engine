#pragma once
/********************************************
*  contents:   vertex shader loader
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
#include "graphics/graphics_types.h"
/******* end header inclusion *******/

namespace Core
{
   struct VertexShader;

   struct VertexShaderLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      VertexShader load(InputLayout layout, const char* buffer, uint32_t bufferSize) const;

      void unload(VertexShader& data);
      
   private:
      ID3D11Device* m_dev;
   };
}
