#pragma once
/********************************************
*  contents:   DXShaderFileLoader
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class DXVertexShader;
   class DXPixelShader;

   class DXShaderFileLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      DXVertexShader loadVertexShader(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied) const;
      DXPixelShader loadPixelShader(const std::string& filename) const;

      void unload(DXPixelShader& shader);
      void unload(DXVertexShader& shader);

   private:
      ID3D11Device* m_dev;
   };
}
