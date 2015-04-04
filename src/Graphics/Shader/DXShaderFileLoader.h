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
   class DXShader;

   class DXShaderFileLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      DXShader load(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied) const;

      void unload(DXShader& shader);

   private:
      DXShaderLoader m_loader;
   };
}
