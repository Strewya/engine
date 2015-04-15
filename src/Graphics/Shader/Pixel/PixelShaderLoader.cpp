//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/Pixel/PixelShaderLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/Shader/Pixel/PixelShader.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool PixelShaderLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(PixelShaderLoader);

      m_dev = device;

      CORE_STATUS_AND(m_dev != nullptr);

      CORE_INIT_END(PixelShaderLoader);
   }

   bool PixelShaderLoader::shutdown()
   {
      CORE_SHUTDOWN_START(PixelShaderLoader);

      m_dev = nullptr;

      CORE_SHUTDOWN_END(PixelShaderLoader);
   }

   PixelShader PixelShaderLoader::load(const char* buffer, uint32_t bufferSize) const
   {
      PixelShader result{nullptr};
      if( buffer != nullptr && bufferSize > 0 )
      {
         HRESULT hr = m_dev->CreatePixelShader(buffer, bufferSize, nullptr, &result.pixel);
         if( FAILED(hr) )
         {
            CORE_INFO("Failed to create pixel shader");
         }
      }
      else
      {
         CORE_INFO("Received invalid pixel shader buffer or invalid buffer size");
      }
      return result;
   }

   void PixelShaderLoader::unload(PixelShader& data)
   {
      safeRelease(data.pixel);
   }
}
