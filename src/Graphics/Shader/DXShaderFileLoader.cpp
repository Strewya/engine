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
      CORE_INIT_START(DXShaderFileLoader);

      CORE_STATUS_AND(m_loader.init(device));

      CORE_INIT_END(DXShaderFileLoader);
   }

   bool DXShaderFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(DXShaderFileLoader);

      CORE_STATUS_AND(m_loader.shutdown());

      CORE_SHUTDOWN_END(DXShaderFileLoader);
   }

   static bool loadFile(const std::string& filename, std::vector<char>& shaderBuffer)
   {
      size_t shaderSize = 0;
      std::ifstream shaderFile{filename, std::ifstream::in | std::ifstream::binary};
      if( shaderFile.good() )
      {
         shaderFile.seekg(0, std::ios::end);
         shaderSize = SIZE_T(shaderFile.tellg());
         shaderBuffer.clear();
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
      std::vector<char> vsBuffer;
      if( loadFile(filename, vsBuffer) )
      {
         result = m_loader.loadVertexShader(ied, vsBuffer.data(), vsBuffer.size());
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
      std::vector<char> psBuffer;
      if( loadFile(filename, psBuffer) )
      {
         result = m_loader.loadPixelShader(psBuffer.data(), psBuffer.size());
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void DXShaderFileLoader::unload(DXVertexShader& shader)
   {
      m_loader.unload(shader);
   }

   void DXShaderFileLoader::unload(DXPixelShader& shader)
   {
      m_loader.unload(shader);
   }
}
