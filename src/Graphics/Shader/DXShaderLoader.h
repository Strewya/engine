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
/******* end header inclusion *******/

namespace Core
{
   class DXVertexShader;
   class DXPixelShader;

   class DXShaderLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      DXVertexShader loadVertexShader(const char* buffer, uint32_t bufferSize, std::vector<D3D11_INPUT_ELEMENT_DESC> ied) const;
      DXPixelShader loadPixelShader(const char* buffer, uint32_t bufferSize) const;

      void unload(DXPixelShader& shader);
      void unload(DXVertexShader& shader);

   private:
      ID3D11Device* m_dev;
   };
}
