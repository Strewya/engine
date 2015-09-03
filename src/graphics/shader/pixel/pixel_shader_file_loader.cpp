//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/shader/pixel/pixel_shader_file_loader.h"
/******* c++ headers *******/
#include <vector>
/******* extra headers *******/
#include "utility/file_loader.h"
#include "graphics/shader/pixel/pixel_shader.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   bool PixelShaderFileLoader::init(PixelShaderLoader& loader)
   {
      CORE_INIT_START(PixelShaderFileLoader);

      m_loader = &loader;

      CORE_STATUS_AND(m_loader != nullptr);

      CORE_INIT_END;
   }

   bool PixelShaderFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(PixelShaderFileLoader);

      CORE_SHUTDOWN_END;
   }

   PixelShader PixelShaderFileLoader::load(const char* filename) const
   {
      PixelShader result{nullptr};
      u8 buffer[1024];
      u32 bufferSize = 1024;
      if( loadFile(filename, buffer, bufferSize) == LoadFileResult::OK )
      {
         result = m_loader->load((const char*)buffer, bufferSize);
      }
      else
      {
         CORE_LOG("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void PixelShaderFileLoader::unload(PixelShader& data)
   {
      m_loader->unload(data);
   }
}
