//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/DXShaderLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/Shader/DXShader.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXShaderLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(DXShaderLoader);

      m_dev = device;

      CORE_STATUS_AND(m_dev != nullptr);

      CORE_INIT_END(DXShaderLoader);
   }

   bool DXShaderLoader::shutdown()
   {
      CORE_SHUTDOWN_START(DXShaderLoader);

      m_dev = nullptr;

      CORE_SHUTDOWN_END(DXShaderLoader);
   }

   DXVertexShader DXShaderLoader::loadVertexShader(InputLayout layout, const char* buffer, uint32_t bufferSize) const
   {
      DXVertexShader result{nullptr, nullptr};
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

   DXPixelShader DXShaderLoader::loadPixelShader(const char* buffer, uint32_t bufferSize) const
   {
      DXPixelShader result{nullptr};
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

   void DXShaderLoader::unload(DXVertexShader& data)
   {
      safeRelease(data.inputLayout);
      safeRelease(data.vertex);
   }

   void DXShaderLoader::unload(DXPixelShader& data)
   {
      safeRelease(data.pixel);
   }
}
