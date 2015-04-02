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
      m_dev = device;

      CORE_STATUS(m_dev != nullptr);
      CORE_INIT(DXShaderLoader);
   }

   bool DXShaderLoader::shutdown()
   {
      m_dev = nullptr;

      CORE_STATUS(true);
      CORE_SHUTDOWN(DXShaderLoader);
   }

   DXVertexShader DXShaderLoader::loadVertexShader(const char* shaderBuffer, uint32_t shaderSize,
                                                   std::vector<D3D11_INPUT_ELEMENT_DESC> ied) const
   {
      DXVertexShader result{nullptr, nullptr};

      if( shaderBuffer != nullptr && shaderSize > 0 )
      {
         HRESULT hr = m_dev->CreateVertexShader(shaderBuffer, shaderSize, nullptr, &result.shader);
         if( SUCCEEDED(hr) )
         {
            hr = m_dev->CreateInputLayout(ied.data(), ied.size(), shaderBuffer, shaderSize, &result.inputLayout);
            if( FAILED(hr) )
            {
               CORE_INFO("Failed to create input layout for vertex shader");
            }
         }
         else
         {
            CORE_INFO("Failed to create vertex shader");
         }
      }
      else
      {
         CORE_INFO("Received invalid vertex shader buffer or buffer size");
      }
      return result;
   }

   DXPixelShader DXShaderLoader::loadPixelShader(const char* shaderBuffer, uint32_t shaderSize) const
   {
      DXPixelShader result{nullptr};

      if( shaderBuffer != nullptr && shaderSize > 0 )
      {
         HRESULT hr = m_dev->CreatePixelShader(shaderBuffer, shaderSize, nullptr, &result.shader);
         if( FAILED(hr) )
         {
            CORE_INFO("Failed to create pixel shader");
         }
      }
      else
      {
         CORE_INFO("Received invalid pixel shader buffer or buffer size");
      }
      return result;
   }

   void DXShaderLoader::unload(DXPixelShader& shader)
   {
      safeRelease(shader.shader);
   }

   void DXShaderLoader::unload(DXVertexShader& shader)
   {
      safeRelease(shader.shader);
      safeRelease(shader.inputLayout);
   }
}
