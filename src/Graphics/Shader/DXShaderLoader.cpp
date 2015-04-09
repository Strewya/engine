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

   DXVertexShader DXShaderLoader::loadVertexShader(InputLayout layout, const char* vsBuffer, uint32_t vsSize) const
   {
      DXVertexShader result{nullptr, nullptr};
      if( vsBuffer != nullptr && vsSize > 0 )
      {
         HRESULT hr = m_dev->CreateVertexShader(vsBuffer, vsSize, nullptr, &result.vertex);
         if( SUCCEEDED(hr) )
         {
            if( !layout.empty() )
            {
               hr = m_dev->CreateInputLayout(layout.data(), layout.size(), vsBuffer, vsSize, &result.inputLayout);
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

   DXPixelShader DXShaderLoader::loadPixelShader(const char* shaderBuffer, uint32_t shaderSize) const
   {
      DXPixelShader result{nullptr};
      if( shaderBuffer != nullptr && shaderSize > 0 )
      {
         HRESULT hr = m_dev->CreatePixelShader(shaderBuffer, shaderSize, nullptr, &result.pixel);
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

   void DXShaderLoader::unload(DXVertexShader& shader)
   {
      safeRelease(shader.inputLayout);
      safeRelease(shader.vertex);
   }

   void DXShaderLoader::unload(DXPixelShader& shader)
   {
      safeRelease(shader.pixel);
   }
}
