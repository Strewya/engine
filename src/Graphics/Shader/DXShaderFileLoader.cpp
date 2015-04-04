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

   DXShader DXShaderFileLoader::load(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied) const
   {
      DXShader result{nullptr, nullptr, nullptr};
      std::vector<char> vsBuffer;
      std::vector<char> psBuffer;
      std::string pixelShaderFilename = filename + "_ps.cso";
      std::string vertexShaderFilename = filename + "_vs.cso";
      if( loadFile(vertexShaderFilename, vsBuffer) && loadFile(pixelShaderFilename, psBuffer) )
      {
         result = m_loader.load(ied, vsBuffer.data(), vsBuffer.size(), psBuffer.data(), psBuffer.size());
      }
      else
      {
         CORE_INFO("Failed to load shader '", pixelShaderFilename, "' or '", vertexShaderFilename, "'");
      }
      return result;
   }

   void DXShaderFileLoader::unload(DXShader& shader)
   {
      m_loader.unload(shader);
   }
}
