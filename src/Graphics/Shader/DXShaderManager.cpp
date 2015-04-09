//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/DXShaderManager.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXShaderManager::init(ID3D11Device* device, DXVertexShader defaultVertexShader, DXPixelShader defaultPixelShader)
   {
      CORE_INIT_START(DXShaderManager);

      m_defaultVertexShader = defaultVertexShader;
      m_defaultPixelShader = defaultPixelShader;

      CORE_STATUS_AND(m_fileloader.init(device));
      CORE_STATUS_AND(m_defaultVertexShader.inputLayout != nullptr);
      CORE_STATUS_AND(m_defaultVertexShader.vertex != nullptr);
      CORE_STATUS_AND(m_defaultPixelShader.pixel != nullptr);

      CORE_INIT_END(DXShaderManager);
   }

   bool DXShaderManager::shutdown()
   {
      CORE_SHUTDOWN_START(DXShaderManager);

      //unload all existing shaders
      m_fileloader.unload(m_defaultVertexShader);
      m_fileloader.unload(m_defaultPixelShader);

      //to not have memory leaks on shutdown, but still check if all resources were cleaned up in the game code
      for( auto& data : m_vertexData )
      {
         m_fileloader.unload(data);
      }
      for( auto& data : m_pixelData )
      {
         m_fileloader.unload(data);
      }

      CORE_STATUS_AND(m_defaultVertexShader.inputLayout == nullptr);
      CORE_STATUS_AND(m_defaultVertexShader.vertex == nullptr);
      CORE_STATUS_AND(m_defaultPixelShader.pixel == nullptr);
      CORE_STATUS_AND(m_vertexData.hasUsedHandles() == false);
      CORE_STATUS_AND(m_pixelData.hasUsedHandles() == false);
      CORE_STATUS_AND(m_fileloader.shutdown());

      CORE_SHUTDOWN_END(DXShaderManager);
   }

   HVertexShader DXShaderManager::loadVertexShaderFromFile(const std::string& filename, InputLayout ied)
   {
      HVertexShader handle = m_vertexNames.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.loadVertexShader(filename, std::move(ied));
         if( loadedData.inputLayout != nullptr && loadedData.vertex != nullptr )
         {
            auto& data = m_vertexData.acquire(handle);
            data = loadedData;
            m_vertexNames.bind(filename, handle);
         }
      }
      return handle;
   }

   HPixelShader DXShaderManager::loadPixelShaderFromFile(const std::string& filename)
   {
      HPixelShader handle = m_pixelNames.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.loadPixelShader(filename);
         if( loadedData.pixel != nullptr )
         {
            auto& data = m_pixelData.acquire(handle);
            data = loadedData;
            m_pixelNames.bind(filename, handle);
         }
      }
      return handle;
   }

   DXVertexShader& DXShaderManager::getData(HVertexShader handle)
   {
      auto* data = m_vertexData.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultVertexShader;
      }
      return *data;
   }

   DXPixelShader& DXShaderManager::getData(HPixelShader handle)
   {
      auto* data = m_pixelData.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultPixelShader;
      }
      return *data;
   }

   void DXShaderManager::release(HVertexShader handle)
   {
      auto* data = m_vertexData.dereference(handle);
      if( data != nullptr )
      {
         m_fileloader.unload(*data);
         m_vertexNames.unbind(handle);
         m_vertexData.release(handle);
      }
   }

   void DXShaderManager::release(HPixelShader handle)
   {
      auto* data = m_pixelData.dereference(handle);
      if( data != nullptr )
      {
         m_fileloader.unload(*data);
         m_pixelNames.unbind(handle);
         m_pixelData.release(handle);
      }
   }
}