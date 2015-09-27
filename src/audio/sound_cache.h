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
      // [Struja 26.9.2015.] #todo: Replace this raw array with a pool allocator based container so i can more easily track how much sounds are
      // actually in use
      Allocator* m_allocator;
      Sound* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;

   public:
      void init(Allocator& a, u32 maxSlots)
      {
         m_allocator = &a;

         m_buffer = m_allocator->allocateArray<Sound>(maxSlots);
         m_maxSlots = maxSlots;
         m_usedSlots = 0;
      }
      void shutdown()
      {
         m_allocator->deallocateArray(m_buffer);
      }

      HSound insert(Sound s)
      {
         HSound result{};

         if( m_maxSlots != m_usedSlots )
         {
            // #todo maybe some day when this becomes a long loop change to a pool based freelist finder or something.
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

      Sound getData(HSound handle)
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
