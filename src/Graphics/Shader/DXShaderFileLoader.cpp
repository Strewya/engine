//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/DXShaderFileLoader.h>
/******* C++ headers *******/
#include <fstream>
/******* extra headers *******/
#include <Graphics/Shader/DXShader.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXShaderFileLoader::init(ID3D11Device* device)
   {
      CORE_STATUS(m_loader.init(device));
      CORE_INIT(DXShaderFileLoader);
   }

   bool DXShaderFileLoader::shutdown()
   {
      CORE_STATUS(m_loader.shutdown());
      CORE_SHUTDOWN(DXShaderFileLoader);
   }

   static bool loadFile(const std::string& filename, std::vector<char>& shaderBuffer)
   {
      size_t shaderSize = 0;
      std::ifstream shaderFile{filename, std::ifstream::in | std::ifstream::binary};
      if( shaderFile.good() )
      {
         shaderFile.seekg(0, std::ios::end);
         shaderSize = SIZE_T(shaderFile.tellg());
         shaderBuffer.resize(shaderSize);
         shaderFile.seekg(0, std::ios::beg);
         shaderFile.read(&shaderBuffer[0], shaderSize);
         shaderFile.close();
         return true;
      }
      return false;
   }

   DXVertexShader DXShaderFileLoader::loadVertexShader(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied) const
   {
      DXVertexShader result{nullptr, nullptr};
      std::vector<char> shaderBuffer;
      if( loadFile(filename, shaderBuffer) )
      {
         result = m_loader.loadVertexShader(shaderBuffer.data(), shaderBuffer.size(), ied);
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   DXPixelShader DXShaderFileLoader::loadPixelShader(const std::string& filename) const
   {
      DXPixelShader result{nullptr};
      std::vector<char> shaderBuffer;
      if( loadFile(filename, shaderBuffer) )
      {
         result = m_loader.loadPixelShader(shaderBuffer.data(), shaderBuffer.size());
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void DXShaderFileLoader::unload(DXPixelShader& shader)
   {
      m_loader.unload(shader);
   }

   void DXShaderFileLoader::unload(DXVertexShader& shader)
   {
      m_loader.unload(shader);
   }
}
