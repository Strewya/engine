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
   struct tag_DXPixelShader;
   typedef Handle<tag_DXPixelShader> HPixelShader;

   struct tag_DXVertexShader;
   typedef Handle<tag_DXVertexShader> HVertexShader;

   class DXShaderManager
   {
   public:
      bool init(ID3D11Device* device, DXPixelShader defaultPixelShader, DXVertexShader defaultVertexShader);
      bool shutdown();

      HPixelShader loadPixelShaderFromFile(const std::string& filename);
      HVertexShader loadVertexShaderFromFile(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied);

      DXPixelShader& getData(HPixelShader handle);
      DXVertexShader& getData(HVertexShader handle);
      void release(HPixelShader handle);
      void release(HVertexShader handle);

   private:
      typedef Cache<DXPixelShader, HPixelShader> PixelShaderCache;
      typedef NameCache<HPixelShader> PixelShaderNames;
      typedef Cache<DXVertexShader, HVertexShader> VertexShaderCache;
      typedef NameCache<HVertexShader> VertexShaderNames;

      PixelShaderCache m_pixelShaderData;
      PixelShaderNames m_pixelShaderNames;

      VertexShaderCache m_vertexShaderData;
      VertexShaderNames m_vertexShaderNames;

      DXPixelShader m_defaultPixelShader;
      DXVertexShader m_defaultVertexShader;

      DXShaderFileLoader m_fileloader;
   };
}
