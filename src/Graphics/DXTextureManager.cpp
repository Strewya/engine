//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXTextureManager.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXTextureManager::init(ID3D11Device* device, DXTexture defaultData)
   {
      m_defaultTexture = defaultData;

      CORE_STATUS(m_fileloader.init(device));
      CORE_STATUS_AND(m_defaultTexture.shaderResourceView != nullptr);

      CORE_INIT(DXTextureManager);
   }

   bool DXTextureManager::shutdown()
   {
      //unload all existing textures
      m_fileloader.unload(m_defaultTexture);
      
      CORE_STATUS(m_defaultTexture.shaderResourceView == nullptr);
      CORE_STATUS_AND(m_data.hasUsedHandles() == false);
      CORE_STATUS_AND(m_fileloader.shutdown());
      CORE_SHUTDOWN(DXTextureManager);
   }

   HTexture DXTextureManager::loadFromFile(const std::string& filename)
   {
      HTexture handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.load(filename);
         if( loadedData.shaderResourceView != nullptr && loadedData.width != 0 && loadedData.height != 0 )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
      }
      return handle;
   }

   DXTexture& DXTextureManager::getData(HTexture handle)
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultTexture;
      }
      return *data;
   }

   void DXTextureManager::release(HTexture handle)
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
