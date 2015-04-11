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
#include <Graphics/Shader/DXShaderHandle.h>
#include <Util/Cache.h>
#include <Util/NameCache.h>
/******* end header inclusion *******/

namespace Core
{
   class DXShaderManager
   {
   public:
      bool init(ID3D11Device* device, DXVertexShader defaultVertexShader, DXPixelShader defaultPixelShader);
      bool shutdown();

      HVertexShader loadVertexShaderFromFile(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> layout);
      HPixelShader loadPixelShaderFromFile(const std::string& filename);

      DXVertexShader& getData(HVertexShader handle);
      DXPixelShader& getData(HPixelShader handle);

      void release(HVertexShader handle);
      void release(HPixelShader handle);

   private:
      typedef Cache<DXVertexShader, HVertexShader> VertexShaderCache;
      typedef NameCache<HVertexShader> VertexShaderNames;
      typedef Cache<DXPixelShader, HPixelShader> PixelShaderCache;
      typedef NameCache<HPixelShader> PixelShaderNames;

      VertexShaderCache m_vertexData;
      VertexShaderNames m_vertexNames;
      DXVertexShader m_defaultVertexShader;

      PixelShaderCache m_pixelData;
      PixelShaderNames m_pixelNames;
      DXPixelShader m_defaultPixelShader;

      DXShaderFileLoader m_fileloader;
   };
}
