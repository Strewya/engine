//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Shader/Pixel/PixelShaderManager.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool PixelShaderManager::init(ID3D11Device* device, PixelShader defaultData)
   {
      CORE_INIT_START(PixelShaderManager);

      m_defaultData = defaultData;
      
      CORE_STATUS_AND(m_fileloader.init(device));
      CORE_STATUS_AND(m_defaultData.pixel != nullptr);

      CORE_INIT_END(PixelShaderManager);
   }

   bool PixelShaderManager::shutdown()
   {
      CORE_SHUTDOWN_START(PixelShaderManager);

      //unload all existing shaders
      m_fileloader.unload(m_defaultData);

      //to not have memory leaks on shutdown, but still check if all resources were cleaned up in the game code
      for( auto& data : m_data )
      {
         m_fileloader.unload(data);
      }

      CORE_STATUS_AND(m_defaultData.pixel == nullptr);
      CORE_STATUS_AND(m_data.hasUsedHandles() == false);
      CORE_STATUS_AND(m_fileloader.shutdown());

      CORE_SHUTDOWN_END(PixelShaderManager);
   }

   HPixelShader PixelShaderManager::loadFromFile(const std::string& filename)
   {
      HPixelShader handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.loadPixelShader(filename);
         if( loadedData.pixel != nullptr )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
      }
      return handle;
   }

   PixelShader& PixelShaderManager::getData(HPixelShader handle)
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultData;
      }
      return *data;
   }

   void PixelShaderManager::release(HPixelShader handle)
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