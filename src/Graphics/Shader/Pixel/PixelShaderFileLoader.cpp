//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/Pixel/PixelShaderFileLoader.h>
/******* C++ headers *******/
#include <fstream>
/******* extra headers *******/
#include <Graphics/Shader/FileLoader.h>
#include <Graphics/Shader/Pixel/PixelShader.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool PixelShaderFileLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(PixelShaderFileLoader);

      CORE_STATUS_AND(m_loader.init(device));

      CORE_INIT_END(PixelShaderFileLoader);
   }

   bool PixelShaderFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(PixelShaderFileLoader);

      CORE_STATUS_AND(m_loader.shutdown());

      CORE_SHUTDOWN_END(PixelShaderFileLoader);
   }

   PixelShader PixelShaderFileLoader::loadPixelShader(const std::string& filename) const
   {
      PixelShader result{nullptr};
      std::vector<char> buffer;
      if( loadFile(filename, buffer) )
      {
         result = m_loader.load(buffer.data(), buffer.size());
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void PixelShaderFileLoader::unload(PixelShader& data)
   {
      m_loader.unload(data);
   }
}
