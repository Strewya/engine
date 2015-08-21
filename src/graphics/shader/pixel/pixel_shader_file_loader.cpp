//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/shader/pixel/pixel_shader_file_loader.h"
/******* c++ headers *******/
#include <vector>
/******* extra headers *******/
#include "graphics/shader/file_loader.h"
#include "graphics/shader/pixel/pixel_shader.h"
#include "util/utility.h"
/******* end headers *******/

namespace core
{
   bool PixelShaderFileLoader::init(PixelShaderLoader& loader)
   {
      CORE_INIT_START(PixelShaderFileLoader);

      m_loader = &loader;

      CORE_STATUS_AND(m_loader != nullptr);

      CORE_INIT_END(PixelShaderFileLoader);
   }

   bool PixelShaderFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(PixelShaderFileLoader);

      CORE_SHUTDOWN_END(PixelShaderFileLoader);
   }

   PixelShader PixelShaderFileLoader::load(const char* filename) const
   {
      PixelShader result{nullptr};
      std::vector<char> buffer;
      if( loadFile(filename, buffer) )
      {
         result = m_loader->load(buffer.data(), buffer.size());
      }
      else
      {
         /* #log */ CORE_LOG("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void PixelShaderFileLoader::unload(PixelShader& data)
   {
      m_loader->unload(data);
   }
}
