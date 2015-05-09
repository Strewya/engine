//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/shader/vertex/vertex_shader_file_loader.h"
/******* c++ headers *******/
#include <vector>
/******* extra headers *******/
#include "graphics/shader/file_loader.h"
#include "graphics/shader/vertex/vertex_shader.h"
#include "util/utility.h"
/******* end headers *******/

namespace core
{
   bool VertexShaderFileLoader::init(VertexShaderLoader& loader)
   {
      CORE_INIT_START(VertexShaderFileLoader);

      m_loader = &loader;

      CORE_STATUS_AND(m_loader != nullptr);

      CORE_INIT_END(VertexShaderFileLoader);
   }

   bool VertexShaderFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(VertexShaderFileLoader);

      CORE_SHUTDOWN_END(VertexShaderFileLoader);
   }

   VertexShader VertexShaderFileLoader::load(const char* filename, InputLayout layout) const
   {
      VertexShader result{nullptr, nullptr};
      std::vector<char> buffer;
      if( loadFile(filename, buffer) )
      {
         result = m_loader->load(layout, buffer.data(), buffer.size());
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void VertexShaderFileLoader::unload(VertexShader& data)
   {
      m_loader->unload(data);
   }
}
