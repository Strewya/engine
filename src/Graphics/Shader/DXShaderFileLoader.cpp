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

   static bool loadFile(const std::string& filename, std::vector<char>& buffer)
   {
      size_t shaderSize = 0;
      std::ifstream shaderFile{filename, std::ifstream::in | std::ifstream::binary};
      if( shaderFile.good() )
      {
         shaderFile.seekg(0, std::ios::end);
         shaderSize = SIZE_T(shaderFile.tellg());
         buffer.clear();
         buffer.resize(shaderSize);
         shaderFile.seekg(0, std::ios::beg);
         shaderFile.read(&buffer[0], shaderSize);
         shaderFile.close();
         return true;
      }
      return false;
   }

   DXVertexShader DXShaderFileLoader::loadVertexShader(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> layout) const
   {
      DXVertexShader result{nullptr, nullptr};
      std::vector<char> buffer;
      if( loadFile(filename, buffer) )
      {
         result = m_loader.loadVertexShader(layout, buffer.data(), buffer.size());
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
      std::vector<char> buffer;
      if( loadFile(filename, buffer) )
      {
         result = m_loader.loadPixelShader(buffer.data(), buffer.size());
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void DXShaderFileLoader::unload(DXVertexShader& data)
   {
      m_loader.unload(data);
   }

   void DXShaderFileLoader::unload(DXPixelShader& data)
   {
      m_loader.unload(data);
   }
}
