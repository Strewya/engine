#pragma once
/********************************************
*  contents:   DXShaderFileLoader
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Graphics/Shader/Pixel/PixelShaderLoader.h>
/******* end header inclusion *******/

namespace Core
{
   class PixelShader;

   class PixelShaderFileLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      PixelShader loadPixelShader(const std::string& filename) const;

      void unload(PixelShader& data);

   private:
      PixelShaderLoader m_loader;
   };
}
