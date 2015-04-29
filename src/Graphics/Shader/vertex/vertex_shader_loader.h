#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
#include "graphics/graphics_typedefs.h"
/******* end header inclusion *******/

namespace Core
{
   class VertexShader;

   class VertexShaderLoader
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
