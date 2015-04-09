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

      DXVertexShader loadVertexShader(InputLayout layout, const char* vsBuffer, uint32_t vsSize) const;
      DXPixelShader loadPixelShader(const char* vsBuffer, uint32_t vsSize) const;

      void unload(DXVertexShader& shader);
      void unload(DXPixelShader& shader);

   private:
      ID3D11Device* m_dev;
   };
}
