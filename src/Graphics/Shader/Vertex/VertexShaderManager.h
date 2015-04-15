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
#include <Graphics/Shader/Vertex/VertexShader.h>
#include <Graphics/Shader/Vertex/VertexShaderFileLoader.h>
#include <Util/Cache.h>
#include <Util/NameCache.h>
/******* end header inclusion *******/

namespace Core
{
   class VertexShaderManager
   {
   public:
      bool init(ID3D11Device* device, VertexShader defaultData);
      bool shutdown();

      HVertexShader loadFromFile(const std::string& filename, InputLayout layout);

      VertexShader& getData(HVertexShader handle);
      
      void release(HVertexShader handle);
      
   private:
      typedef Cache<VertexShader, HVertexShader> ShaderCache;
      typedef NameCache<HVertexShader> ShaderNames;
      
      ShaderCache m_data;
      ShaderNames m_names;
      VertexShader m_defaultData;

      VertexShaderFileLoader m_fileloader;
   };
}
