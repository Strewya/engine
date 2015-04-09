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
#include <Graphics/Shader/DXShaderLoader.h>
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

      DXVertexShader loadVertexShader(const std::string& filename, InputLayout ied) const;
      DXPixelShader loadPixelShader(const std::string& filename) const;

      void unload(DXVertexShader& shader);
      void unload(DXPixelShader& shader);

   private:
      DXShaderLoader m_loader;
   };
}
