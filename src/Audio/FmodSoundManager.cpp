//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Audio/FmodSoundManager.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool FmodSoundManager::init(FMOD::System* system, FmodSound defaultSound)
   {
      CORE_INIT_START(FmodSoundManager);

      m_defaultData = defaultSound;

      CORE_STATUS_AND(m_fileloader.init(system));
      //CORE_STATUS_AND(m_defaultData.sound != nullptr);

      CORE_INIT_END(FmodSoundManager);
   }

   bool FmodSoundManager::shutdown()
   {
      CORE_SHUTDOWN_START(FmodSoundManager);

      //unload all existing Sounds
      m_fileloader.unload(m_defaultData);
      //to not have memory leaks on shutdown, but still check if all resources were cleaned up by the game code
      for( auto& data : m_data )
      {
         m_fileloader.unload(data);
      }

      CORE_STATUS_AND(m_defaultData.sound == nullptr);
      CORE_STATUS_AND(m_data.hasUsedHandles() == false);
      CORE_STATUS_AND(m_fileloader.shutdown());

      CORE_SHUTDOWN_END(FmodSoundManager);
   }

   HSound FmodSoundManager::loadFromFile(const std::string& filename)
   {
      HSound handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_fileloader.load(filename);
         if( loadedData.sound != nullptr )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
      }
      return handle;
   }

   FmodSound& FmodSoundManager::getData(HSound handle)
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultData;
      }
      return *data;
   }

   void FmodSoundManager::release(HSound handle)
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
