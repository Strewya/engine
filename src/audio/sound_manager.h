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
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct LinearAllocator;

   struct SoundCache
   {
      bool init(LinearAllocator& allocator, u32 soundCount)
      {
         CORE_INIT_START(SoundCache);

         m_soundCount = 0;
         m_maxSounds = soundCount;
         m_soundsBuffer = allocate<Sound>(allocator, soundCount);
         m_freelist.init(m_soundsBuffer, soundCount);

         CORE_INIT_END;
      }

      HSound insert(Sound s)
      {
         HSound result{};
         u8* slot = allocate(m_freelist);
         if( slot )
         {
            Sound* soundSlot = (Sound*)slot;
            *soundSlot = s;
            u16 index = (u16)(soundSlot - m_soundsBuffer);
            result.init(index);
         }
         return result;
      }

      Sound remove(HSound handle)
      {
         Sound s = m_soundsBuffer[handle.getIndex()];
         release(m_freelist, &m_soundsBuffer[handle.getIndex()]);
         return s;
      }

      Sound getData(HSound handle)
      {
         Sound result = m_soundsBuffer[handle.getIndex()];
         return result;
      }

   private:
      Sound* m_soundsBuffer;
      Freelist m_freelist;
      u32 m_maxSounds;
      u32 m_soundCount;
   };

}
