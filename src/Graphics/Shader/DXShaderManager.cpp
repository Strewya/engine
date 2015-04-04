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
   bool DXShaderManager::init(ID3D11Device* device, DXShader defaultShader)
   {
      CORE_INIT_START(DXShaderManager);

      m_defaultShader = defaultShader;

      CORE_STATUS_AND(m_fileloader.init(device));
      CORE_STATUS_AND(m_defaultShader.inputLayout != nullptr);
      CORE_STATUS_AND(m_defaultShader.vertex != nullptr);
      CORE_STATUS_AND(m_defaultShader.pixel != nullptr);

      CORE_INIT_END(DXShaderManager);
   }

   bool DXShaderManager::shutdown()
   {
      CORE_SHUTDOWN_START(DXShaderManager);

      //unload all existing shaders
      m_fileloader.unload(m_defaultShader);

      //to not have memory leaks on shutdown, but still check if all resources were cleaned up in the game code
      for( auto& data : m_data )
      {
         m_fileloader.unload(data);
      }

      CORE_STATUS_AND(m_defaultShader.inputLayout == nullptr);
      CORE_STATUS_AND(m_defaultShader.vertex == nullptr);
      CORE_STATUS_AND(m_defaultShader.pixel == nullptr);
      CORE_STATUS_AND(m_data.hasUsedHandles() == false);
      CORE_STATUS_AND(m_fileloader.shutdown());

      CORE_SHUTDOWN_END(DXShaderManager);
   }

   HShader DXShaderManager::loadFromFile(const std::string& filename, InputLayout ied)
   {
      HShader handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.load(filename, std::move(ied));
         if( loadedData.inputLayout != nullptr && loadedData.vertex != nullptr && loadedData.pixel != nullptr )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
      }
      return handle;
   }

   DXShader& DXShaderManager::getData(HShader handle)
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultShader;
      }
      return *data;
   }

   void DXShaderManager::release(HShader handle)
   {
      auto* data = m_data.dereference(handle);
      if( data != nullptr )
      {
         m_fileloader.unload(*data);
         m_names.unbind(handle);
         m_data.release(handle);
      }
   }
}