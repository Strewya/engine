//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/Vertex/VertexShaderFileLoader.h>
/******* C++ headers *******/
#include <fstream>
/******* extra headers *******/
#include <Graphics/Shader/FileLoader.h>
#include <Graphics/Shader/Vertex/VertexShader.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool VertexShaderFileLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(VertexShaderFileLoader);

      CORE_STATUS_AND(m_loader.init(device));

      CORE_INIT_END(VertexShaderFileLoader);
   }

   bool VertexShaderFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(VertexShaderFileLoader);

      CORE_STATUS_AND(m_loader.shutdown());

      CORE_SHUTDOWN_END(VertexShaderFileLoader);
   }

   VertexShader VertexShaderFileLoader::load(const std::string& filename, InputLayout layout) const
   {
      VertexShader result{nullptr, nullptr};
      std::vector<char> buffer;
      if( loadFile(filename, buffer) )
      {
         result = m_loader.load(layout, buffer.data(), buffer.size());
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void VertexShaderFileLoader::unload(VertexShader& data)
   {
      m_loader.unload(data);
   }
}
