//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/shader/vertex/vertex_shader_loader.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/shader/vertex/vertex_shader.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   void VertexShaderLoader::init(ID3D11Device* device)
   {
      m_dev = device;
      CORE_ASSERT_DBGERR(m_dev != nullptr, "D3D11 device is NULL in VertexShaderLoader!");
   }

   VertexShader VertexShaderLoader::load(InputLayout layout, void* buffer, u32 bufferSize) const
   {
      VertexShader result{nullptr, nullptr};
      if( buffer != nullptr && bufferSize > 0 )
      {
         HRESULT hr = m_dev->CreateVertexShader(buffer, bufferSize, nullptr, &result._vertex);
         if( SUCCEEDED(hr) )
         {
            hr = m_dev->CreateInputLayout(layout.buffer, layout.size, buffer, bufferSize, &result._inputLayout);
            if( FAILED(hr) )
            {
               CORE_LOG("Failed to create input layout for vertex shader");
            }
         }
         else
         {
            CORE_LOG("Failed to create vertex shader");
         }
      }
      else
      {
         CORE_LOG("Received invalid vertex shader buffer or invalid buffer size");
      }
      return result;
   }

   void VertexShaderLoader::unload(VertexShader& data)
   {
      safeRelease(data._inputLayout);
      safeRelease(data._vertex);
   }
}
