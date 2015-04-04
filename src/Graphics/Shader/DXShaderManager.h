#pragma once
/********************************************
*  contents:   DXShaderManager
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Graphics/Shader/DXShader.h>
#include <Graphics/Shader/DXShaderFileLoader.h>
#include <Util/Cache.h>
#include <Util/Handle.h>
#include <Util/NameCache.h>
/******* end header inclusion *******/

namespace Core
{
   struct tag_DXShader;
   typedef Handle<tag_DXShader> HShader;

   class DXShaderManager
   {
   public:
      bool init(ID3D11Device* device, DXShader defaultShader);
      bool shutdown();

      HShader loadFromFile(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied);
      DXShader& getData(HShader handle);
      void release(HShader handle);

   private:
      typedef Cache<DXShader, HShader> ShaderCache;
      typedef NameCache<HShader> ShaderNames;

      ShaderCache m_data;
      ShaderNames m_names;
      DXShader m_defaultShader;

      DXShaderFileLoader m_fileloader;
   };
}
