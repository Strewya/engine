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
   class PixelShader;

   typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayout;

   class PixelShaderLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      PixelShader load(const char* buffer, uint32_t bufferSize) const;

      void unload(PixelShader& data);

   private:
      ID3D11Device* m_dev;
   };
}
