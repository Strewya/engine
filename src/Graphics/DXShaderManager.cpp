//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXShaderManager.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXShaderManager::init(ID3D11Device* device, DXPixelShader defaultPixelShader, DXVertexShader defaultVertexShader)
   {
      m_defaultPixelShader = defaultPixelShader;
      m_defaultVertexShader = defaultVertexShader;

      CORE_STATUS(m_fileloader.init(device));
      CORE_STATUS_AND(m_defaultPixelShader.shader != nullptr);
      CORE_STATUS_AND(m_defaultVertexShader.shader != nullptr);

      CORE_INIT(DXShaderManager);
   }

   bool DXShaderManager::shutdown()
   {
      //unload all existing shaders
      m_fileloader.unload(m_defaultPixelShader);
      m_fileloader.unload(m_defaultVertexShader);
      
      CORE_STATUS(m_defaultPixelShader.shader == nullptr);
      CORE_STATUS_AND(m_defaultVertexShader.shader == nullptr);
      CORE_STATUS_AND(m_pixelShaderData.hasUsedHandles() == false);
      CORE_STATUS_AND(m_vertexShaderData.hasUsedHandles() == false);
      CORE_STATUS_AND(m_fileloader.shutdown());
      CORE_SHUTDOWN(DXShaderManager);
   }

   HPixelShader DXShaderManager::loadPixelShaderFromFile(const std::string& filename)
   {
      HPixelShader handle = m_pixelShaderNames.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.loadPixelShader(filename);
         if( loadedData.shader != nullptr )
         {
            auto& data = m_pixelShaderData.acquire(handle);
            data = loadedData;
            m_pixelShaderNames.bind(filename, handle);
         }
      }
      return handle;
   }

   HVertexShader DXShaderManager::loadVertexShaderFromFile(const std::string& filename, std::vector<D3D11_INPUT_ELEMENT_DESC> ied)
   {
      HVertexShader handle = m_vertexShaderNames.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.loadVertexShader(filename, std::move(ied));
         if( loadedData.shader != nullptr )
         {
            auto& data = m_vertexShaderData.acquire(handle);
            data = loadedData;
            m_vertexShaderNames.bind(filename, handle);
         }
      }
      return handle;
   }

   DXPixelShader& DXShaderManager::getData(HPixelShader handle)
   {
      auto* data = m_pixelShaderData.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultPixelShader;
      }
      return *data;
   }

   DXVertexShader& DXShaderManager::getData(HVertexShader handle)
   {
      auto* data = m_vertexShaderData.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultVertexShader;
      }
      return *data;
   }

   void DXShaderManager::release(HPixelShader handle)
   {
      auto* data = m_pixelShaderData.dereference(handle);
      if( data != nullptr )
      {
         m_fileloader.unload(*data);
         m_pixelShaderNames.unbind(handle);
         m_pixelShaderData.release(handle);
      }
   }

   void DXShaderManager::release(HVertexShader handle)
   {
      auto* data = m_vertexShaderData.dereference(handle);
      if( data != nullptr )
      {
         m_fileloader.unload(*data);
         m_vertexShaderNames.unbind(handle);
         m_vertexShaderData.release(handle);
      }
   }
}