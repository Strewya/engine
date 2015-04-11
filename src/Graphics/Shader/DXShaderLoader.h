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

   typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayout;

   class DXShaderLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      DXVertexShader loadVertexShader(InputLayout layout, const char* buffer, uint32_t bufferSize) const;
      DXPixelShader loadPixelShader(const char* buffer, uint32_t bufferSize) const;

      void unload(DXVertexShader& data);
      void unload(DXPixelShader& data);

   private:
      ID3D11Device* m_dev;
   };
}
