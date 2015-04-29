//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/shader/vertex/vertex_shader_loader.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/shader/vertex/vertex_shader.h"
#include "util/utility.h"
/******* end headers *******/

namespace Core
{
   bool VertexShaderLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(VertexShaderLoader);

      m_dev = device;

      CORE_STATUS_AND(m_dev != nullptr);

      CORE_INIT_END(VertexShaderLoader);
   }

   bool VertexShaderLoader::shutdown()
   {
      CORE_SHUTDOWN_START(VertexShaderLoader);

      m_dev = nullptr;

      CORE_SHUTDOWN_END(VertexShaderLoader);
   }

   VertexShader VertexShaderLoader::load(InputLayout layout, const char* buffer, uint32_t bufferSize) const
   {
      VertexShader result{nullptr, nullptr};
      if( buffer != nullptr && bufferSize > 0 )
      {
         HRESULT hr = m_dev->CreateVertexShader(buffer, bufferSize, nullptr, &result.vertex);
         if( SUCCEEDED(hr) )
         {
            if( !layout.empty() )
            {
               hr = m_dev->CreateInputLayout(layout.data(), layout.size(), buffer, bufferSize, &result.inputLayout);
               if( FAILED(hr) )
               {
                  CORE_INFO("Failed to create input layout for vertex shader");
               }
            }
            else
            {
               CORE_INFO("Received empty input layout");
            }
         }
         else
         {
            CORE_INFO("Failed to create vertex shader");
         }
      }
      else
      {
         CORE_INFO("Received invalid vertex shader buffer or invalid buffer size");
      }
      return result;
   }

   void VertexShaderLoader::unload(VertexShader& data)
   {
      safeRelease(data.inputLayout);
      safeRelease(data.vertex);
   }
}
