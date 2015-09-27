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
   void PixelShaderLoader::init(ID3D11Device* device)
   {
      m_dev = device;
      CORE_ASSERT_DBGERR(m_dev != nullptr, "D3D11 device is NULL in PixelShaderLoader");
   }

   PixelShader PixelShaderLoader::load(void* buffer, u32 bufferSize) const
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
