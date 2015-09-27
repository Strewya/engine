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
   private:
      Allocator* m_allocator;
      Texture* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;
   
   public:
      void init(Allocator& a, u32 maxSlots)
      {
         m_allocator = &a;
         m_buffer = m_allocator->allocateArray<Texture>(maxSlots);
         m_maxSlots = maxSlots;
         m_usedSlots = 0;
      }
      void shutdown()
      {
         m_allocator->deallocateArray(m_buffer);
      }

      HTexture insert(Texture t)
      {
         HTexture result{};

         if( m_maxSlots != m_usedSlots )
         {
            for( u16 i = 0; i < m_maxSlots; ++i )
            {
               if( m_buffer[i].unloaded() )
               {
                  m_buffer[i] = t;
                  result.init(i);
                  ++m_usedSlots;
                  break;
               }
            }
         }

         return result;
      }

      Texture remove(HTexture handle)
      {
         Texture t = m_buffer[handle.getIndex()];
         m_buffer[handle.getIndex()] = {};
         --m_usedSlots;
         return t;
      }

      Texture getData(HTexture handle)
      {
         Texture result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_usedSlots;
      }
   };
}
