#pragma once
/********************************************
*  contents:   texture resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/texture/texture.h"
#include "graphics/texture/texture_handle.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct TextureCache
   {
      bool init(LinearAllocator& a, u32 maxSlots)
      {
         CORE_INIT_START(TextureCache);

         m_count = 0;
         m_maxSlots = maxSlots;
         m_buffer = allocate<Texture>(a, maxSlots);

         CORE_INIT_END;
      }

      HTexture insert(Texture t)
      {
         HTexture result{};
         u8* slot = allocate(m_freelist);
         if( slot )
         {
            Texture* textureSlot = (Texture*)slot;
            *textureSlot = t;
            u16 index = u16(textureSlot - m_buffer);
            result.init(index);
            ++m_count;
         }
         return result;
      }

      Texture remove(HTexture handle)
      {
         auto index = handle.getIndex();
         Texture t = m_buffer[index];
         deallocate(m_freelist, &m_buffer[index]);
         --m_count;
         return t;
      }

      Texture getData(HTexture handle)
      {
         Texture result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_count;
      }

   private:
      Texture* m_buffer;
      Freelist m_freelist;
      u32 m_maxSlots;
      u32 m_count;
   };

}
