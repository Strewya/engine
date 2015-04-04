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
   class DXShader;

   typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayout;

   class DXShaderLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      DXShader load(InputLayout layout,
                    const char* vsBuffer, uint32_t vsSize,
                    const char* psBuffer, uint32_t psSize) const;

      void unload(DXShader& shader);

   private:
      ID3D11Device* m_dev;

      ID3D11InputLayout* loadInputLayout(InputLayout layout, const char* vsBuffer, uint32_t vsSize) const;
      ID3D11VertexShader* loadVertexShader(const char* vsBuffer, uint32_t vsSize) const;
      ID3D11PixelShader* loadPixelShader(const char* psBuffer, uint32_t psSize) const;
   };
}
