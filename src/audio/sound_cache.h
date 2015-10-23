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
   private:
      // [Struja 26.9.2015.] #todo: Replace this raw array with a pool allocator based container
      // so i can more easily track how much sounds are actually in use
      Sound* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;

   public:
      void init(Memory& mem, u32 maxSlots)
      {
         m_buffer = emplaceArray<Sound>(mem, maxSlots);
         CORE_ASSERT_DBGERR(m_buffer != nullptr, "Not enough memory to emplace SoundCache storage.");
         m_maxSlots = maxSlots;
         m_usedSlots = 0;
      }

      HSound insert(Sound s)
      {
         HSound result{};

         if( m_maxSlots != m_usedSlots )
         {
            for( u16 i = 0; i < m_maxSlots; ++i )
            {
               if( m_buffer[i].unloaded() )
               {
                  m_buffer[i] = s;
                  result.init(i);
                  ++m_usedSlots;
                  break;
               }
            }
         }
         return result;
      }

      Sound remove(HSound handle)
      {
         Sound result = m_buffer[handle.getIndex()];
         m_buffer[handle.getIndex()] = {};
         --m_usedSlots;
         return result;
      }

      Sound getData(HSound handle) const
      {
         Sound result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_usedSlots;
      }

   };
}
