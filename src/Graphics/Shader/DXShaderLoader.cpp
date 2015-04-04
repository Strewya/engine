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

   DXShader DXShaderLoader::load(InputLayout layout,
                                 const char* vsBuffer, uint32_t vsSize,
                                 const char* psBuffer, uint32_t psSize) const
   {
      return
      {
         loadInputLayout(layout, vsBuffer, vsSize),
         loadVertexShader(vsBuffer, vsSize),
         loadPixelShader(psBuffer, psSize)
      };
   }

   void DXShaderLoader::unload(DXShader& shader)
   {
      safeRelease(shader.inputLayout);
      safeRelease(shader.vertex);
      safeRelease(shader.pixel);
   }

   ID3D11InputLayout* DXShaderLoader::loadInputLayout(InputLayout layout, const char* vsBuffer, uint32_t vsSize) const
   {
      ID3D11InputLayout* result{nullptr};
      if( !layout.empty() && vsBuffer != nullptr && vsSize > 0 )
      {
         HRESULT hr = m_dev->CreateInputLayout(layout.data(), layout.size(), vsBuffer, vsSize, &result);
         if( FAILED(hr) )
         {
            CORE_INFO("Failed to create input layout for vertex shader");
         }
      }
      else
      {
         CORE_INFO("Received empty layout or invalid vertex shader buffer or invalid buffer size");
      }
      return result;
   }

   ID3D11VertexShader* DXShaderLoader::loadVertexShader(const char* vsBuffer, uint32_t vsSize) const
   {
      ID3D11VertexShader* result{nullptr};
      if( vsBuffer != nullptr && vsSize > 0 )
      {
         HRESULT hr = m_dev->CreateVertexShader(vsBuffer, vsSize, nullptr, &result);
         if( FAILED(hr) )
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

   ID3D11PixelShader* DXShaderLoader::loadPixelShader(const char* shaderBuffer, uint32_t shaderSize) const
   {
      ID3D11PixelShader* result{nullptr};
      if( shaderBuffer != nullptr && shaderSize > 0 )
      {
         HRESULT hr = m_dev->CreatePixelShader(shaderBuffer, shaderSize, nullptr, &result);
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
}
