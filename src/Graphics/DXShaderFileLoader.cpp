//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXShaderFileLoader.h>
/******* C++ headers *******/
#include <fstream>
/******* extra headers *******/
#include <Graphics/DXShader.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXShaderFileLoader::init(ID3D11Device* device)
   {
      m_dev = device;
      
      CORE_STATUS(true);
      CORE_INIT(DXShaderFileLoader);
   }

   bool DXShaderFileLoader::shutdown()
   {
      CORE_STATUS(true);
      CORE_SHUTDOWN(DXShaderFileLoader);
   }

   DXVertexShader DXShaderFileLoader::loadVertexShader(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied) const
   {
      DXVertexShader result{filename, nullptr, nullptr};
      SIZE_T shaderSize = 0;
      std::vector<char> shaderBuffer;

      std::ifstream shaderFile{filename, std::ifstream::in | std::ifstream::binary};
      if( shaderFile.good() )
      {
         shaderFile.seekg(0, std::ios::end);
         shaderSize = SIZE_T(shaderFile.tellg());
         shaderBuffer.resize(shaderSize);
         shaderFile.seekg(0, std::ios::beg);
         shaderFile.read(&shaderBuffer[0], shaderSize);
         shaderFile.close();

         HRESULT hr = m_dev->CreateVertexShader(shaderBuffer.data(), shaderSize, nullptr, &result.shader);
         if( SUCCEEDED(hr) )
         {
            hr = m_dev->CreateInputLayout(ied.data(), ied.size(), shaderBuffer.data(), shaderSize, &result.inputLayout);
            if( FAILED(hr) )
            {
               CORE_INFO("Failed to create input layout for vertex shader '", filename, "'");
            }
         }
         else
         {
            CORE_INFO("Failed to create vertex shader '", filename, "'");
         }
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   DXPixelShader DXShaderFileLoader::loadPixelShader(const std::string& filename) const
   {
      DXPixelShader result{filename, nullptr};
      SIZE_T shaderSize = 0;
      std::vector<char> shaderBuffer;
      
      std::ifstream shaderFile{filename, std::ifstream::in | std::ifstream::binary};
      if( shaderFile.good() )
      {
         shaderFile.seekg(0, std::ios::end);
         shaderSize = SIZE_T(shaderFile.tellg());
         shaderBuffer.resize(shaderSize);
         shaderFile.seekg(0, std::ios::beg);
         shaderFile.read(&shaderBuffer[0], shaderSize);
         shaderFile.close();

         HRESULT hr = m_dev->CreatePixelShader(shaderBuffer.data(), shaderSize, nullptr, &result.shader);
         if( FAILED(hr) )
         {
            CORE_INFO("Failed to create pixel shader '", filename, "'");
         }
      }
      else
      {
         CORE_INFO("Failed to load shader '", filename, "'");
      }
      return result;
   }

   void DXShaderFileLoader::unload(DXPixelShader& shader)
   {
      shader.shader->Release();
      shader.shader = nullptr;
   }

   void DXShaderFileLoader::unload(DXVertexShader& shader)
   {
      safeRelease(shader.shader);
      safeRelease(shader.inputLayout);
   }
}
