#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/font/font_descriptor.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct FontCache
   {
   private:
      FontDescriptor* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;

   public:
      void init(Memory& m, u32 maxSlots)
      {
         m_buffer = emplaceArray<FontDescriptor>(m, maxSlots);
         CORE_ASSERT_DBGERR(m_buffer != nullptr, "Not enough memory for FontCache storage!");
         m_maxSlots = maxSlots;
         m_usedSlots = 0;
      }

      HTexture insert(FontDescriptor fd)
      {
         HTexture result{};

         if( m_maxSlots != m_usedSlots )
         {
            for( u16 i = 0; i < m_maxSlots; ++i )
            {
               if( m_buffer[i].unloaded() )
               {
                  m_buffer[i] = fd;
                  result.init(i);
                  ++m_usedSlots;
                  break;
               }
            }
         }

         return result;
      }

      FontDescriptor remove(HFont handle)
      {
         FontDescriptor fd = m_buffer[handle.getIndex()];
         m_buffer[handle.getIndex()] = {};
         --m_usedSlots;
         return fd;
      }

      FontDescriptor& getData(HFont handle) const
      {
         FontDescriptor& result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_usedSlots;
      }
   };
}