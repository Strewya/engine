//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/shader/pixel/pixel_shader_loader.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/shader/pixel/pixel_shader.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   bool PixelShaderLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(PixelShaderLoader);

      m_dev = device;

      CORE_STATUS_AND(m_dev != nullptr);

      CORE_INIT_END;
   }

   PixelShader PixelShaderLoader::load(u8* buffer, u32 bufferSize) const
   {
      PixelShader result{nullptr};
      if( buffer != nullptr && bufferSize > 0 )
      {
         HRESULT hr = m_dev->CreatePixelShader(buffer, bufferSize, nullptr, &result._pixel);
         if( FAILED(hr) )
         {
            CORE_LOG("Failed to create pixel shader");
         }
      }
      else
      {
         CORE_LOG("Received invalid pixel shader buffer or invalid buffer size");
      }
      return result;
   }

   void PixelShaderLoader::unload(PixelShader& data)
   {
      safeRelease(data._pixel);
   }
}
