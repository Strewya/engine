#pragma once
/********************************************
*  contents:   fmod sound resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "audio/sound.h"
#include "audio/sound_handle.h"
#include "audio/sound_file_loader.h"
#include "utility/template/resource_manager_template.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   typedef Manager<Sound, HSound, SoundFileLoader> SoundManager_old;
   
   struct LinearAllocator;

   struct SoundCache
   {
      bool init(LinearAllocator& allocator, u32 soundCount)
      {
         CORE_INIT_START(SoundCache);

         m_soundCount = 0;
         m_maxSounds = soundCount;
         m_sounds = allocate<Sound>(allocator, soundCount);

         CORE_INIT_END;
      }

   private:
      Sound* m_sounds;
      u32 m_maxSounds;
      u32 m_soundCount;
   };





   struct SoundManager
   {
      bool init(LinearAllocator& allocator, SoundFileLoader& fileLoader, u32 soundCount)
      {
         CORE_INIT_START(SoundManager);

         m_soundFileLoader = &fileLoader;
         m_soundCache.init(allocator, soundCount);

         CORE_INIT_END;
      }

   private:
      SoundCache m_soundCache;
      SoundFileLoader* m_soundFileLoader;
   };
}
