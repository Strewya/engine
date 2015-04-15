#pragma once
/********************************************
*  contents:   DXShaderFileLoader
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Graphics/Typedefs.h>
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
