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
#include <graphics/dx_include.h>
/******* extra headers *******/
#include <graphics/graphics_typedefs.h>
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
