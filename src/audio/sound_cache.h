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
   struct SoundCache
   {
      bool init(LinearAllocator& a, u32 maxSlots)
      {
         CORE_INIT_START(SoundCache);

         m_count = 0;
         m_maxSlots = maxSlots;
         m_buffer = allocate<Sound>(a, maxSlots);
         m_freelist.init(m_buffer, maxSlots);

         CORE_INIT_END;
      }

      HSound insert(Sound s)
      {
         HSound result{};
         auto* slot = allocate(m_freelist);
         if( slot )
         {
            Sound* soundSlot = (Sound*)slot;
            *soundSlot = s;
            u16 index = u16(soundSlot - m_buffer);
            result.init(index);
            ++m_count;
         }
         return result;
      }

      Sound remove(HSound handle)
      {
         Sound s = m_buffer[handle.getIndex()];
         deallocate(m_freelist, &m_buffer[handle.getIndex()]);
         --m_count;
         return s;
      }

      Sound getData(HSound handle)
      {
         Sound result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_count;
      }

   private:
      Sound* m_buffer;
      Freelist m_freelist;
      u32 m_maxSlots;
      u32 m_count;
   };
}
