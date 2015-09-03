//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/shader/vertex/vertex_shader_file_loader.h"
/******* c++ headers *******/
#include <vector>
/******* extra headers *******/
#include "utility/file_loader.h"
#include "graphics/shader/vertex/vertex_shader.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   bool VertexShaderFileLoader::init(VertexShaderLoader& loader)
   {
      CORE_INIT_START(VertexShaderFileLoader);

      m_loader = &loader;

      CORE_STATUS_AND(m_loader != nullptr);

      CORE_INIT_END;
   }

   bool VertexShaderFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(VertexShaderFileLoader);

      CORE_SHUTDOWN_END;
   }

   VertexShader VertexShaderFileLoader::load(const char* filename, InputLayout layout) const
   {
      VertexShader result{nullptr, nullptr};
      u8 buffer[1024];
      u32 bufferSize = 1024;
      if( loadFile(filename, buffer, bufferSize) == LoadFileResult::OK )
      {
         result = m_loader->load(layout, (const char*)buffer, bufferSize);
      }
      else
      {
         CORE_LOG("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void VertexShaderFileLoader::unload(VertexShader& data)
   {
      m_loader->unload(data);
   }
}
