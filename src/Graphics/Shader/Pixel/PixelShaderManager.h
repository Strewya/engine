#pragma once
/********************************************
*  contents:   DXShaderManager
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Graphics/Shader/ShaderHandle.h>
#include <Graphics/Shader/Pixel/PixelShader.h>
#include <Graphics/Shader/Pixel/PixelShaderFileLoader.h>
#include <Util/Cache.h>
#include <Util/NameCache.h>
/******* end header inclusion *******/

namespace Core
{
   class PixelShaderManager
   {
   public:
      bool init(ID3D11Device* device, PixelShader defaultData);
      bool shutdown();

      HPixelShader loadFromFile(const std::string& filename);

      PixelShader& getData(HPixelShader handle);

      void release(HPixelShader handle);

   private:
      typedef Cache<PixelShader, HPixelShader> ShaderCache;
      typedef NameCache<HPixelShader> ShaderNames;

      ShaderCache m_data;
      ShaderNames m_names;
      PixelShader m_defaultData;

      PixelShaderFileLoader m_fileloader;
   };
}
