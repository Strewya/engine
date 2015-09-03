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
   bool VertexShaderLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(VertexShaderLoader);

      m_dev = device;

      CORE_STATUS_AND(m_dev != nullptr);

      CORE_INIT_END;
   }

   bool VertexShaderLoader::shutdown()
   {
      CORE_SHUTDOWN_START(VertexShaderLoader);

      m_dev = nullptr;

      CORE_SHUTDOWN_END;
   }

   VertexShader VertexShaderLoader::load(InputLayout layout, const char* buffer, u32 bufferSize) const
   {
      VertexShader result{nullptr, nullptr};
      if( buffer != nullptr && bufferSize > 0 )
      {
         HRESULT hr = m_dev->CreateVertexShader(buffer, bufferSize, nullptr, &result._vertex);
         if( SUCCEEDED(hr) )
         {
            if( !layout.empty() )
            {
               hr = m_dev->CreateInputLayout(layout.data(), (UINT)layout.size(), buffer, bufferSize, &result._inputLayout);
               if( FAILED(hr) )
               {
                  CORE_LOG("Failed to create input layout for vertex shader");
               }
            }
            else
            {
               CORE_LOG("Received empty input layout");
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
