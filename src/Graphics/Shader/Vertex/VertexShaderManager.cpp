//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/Vertex/VertexShaderManager.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool VertexShaderManager::init(ID3D11Device* device, VertexShader defaultData)
   {
      CORE_INIT_START(VertexShaderManager);

      m_defaultData = defaultData;
      
      CORE_STATUS_AND(m_fileloader.init(device));
      CORE_STATUS_AND(m_defaultData.inputLayout != nullptr);
      CORE_STATUS_AND(m_defaultData.vertex != nullptr);

      CORE_INIT_END(VertexShaderManager);
   }

   bool VertexShaderManager::shutdown()
   {
      CORE_SHUTDOWN_START(VertexShaderManager);

      //unload all existing shaders
      m_fileloader.unload(m_defaultData);

      //to not have memory leaks on shutdown, but still check if all resources were cleaned up in the game code
      for( auto& data : m_data )
      {
         m_fileloader.unload(data);
      }

      CORE_STATUS_AND(m_defaultData.inputLayout == nullptr);
      CORE_STATUS_AND(m_defaultData.vertex == nullptr);
      CORE_STATUS_AND(m_data.hasUsedHandles() == false);
      CORE_STATUS_AND(m_fileloader.shutdown());

      CORE_SHUTDOWN_END(VertexShaderManager);
   }

   HVertexShader VertexShaderManager::loadFromFile(const std::string& filename, InputLayout ied)
   {
      HVertexShader handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.load(filename, std::move(ied));
         if( loadedData.inputLayout != nullptr && loadedData.vertex != nullptr )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
      }
      return handle;
   }

   VertexShader& VertexShaderManager::getData(HVertexShader handle)
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultData;
      }
      return *data;
   }

   void VertexShaderManager::release(HVertexShader handle)
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