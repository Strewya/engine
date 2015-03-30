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
   bool DXTextureManager::init(DXTexture defaultData)
   {
      m_default = defaultData;

      CORE_STATUS(true);
      CORE_INIT(DXTextureManager);
   }

   bool DXTextureManager::shutdown()
   {
      CORE_STATUS(true);
      CORE_SHUTDOWN(DXTextureManager);
   }

   HTexture DXTextureManager::loadFromFile(const std::string& filename)
   {
      auto handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.load(filename);
         if( loadedData.shaderResourceView != nullptr && loadedData.width != 0 && loadedData.height != 0 )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
         else
         {
            m_data.release(handle);
            handle = HTexture{};
         }
      }
      return handle;
   }

   DXTexture& DXTextureManager::getData(HTexture handle)
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_default;
      }
      return *data;
   }

   void DXTextureManager::release(HTexture handle)
   {
      if( m_data.dereference(handle) != nullptr )
      {
         m_data.release(handle);
         m_names.unbind(handle);
      }
   }
}
