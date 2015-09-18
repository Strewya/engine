#pragma once
/********************************************
*  contents:   pixel shader resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/shader/shader_handle.h"
#include "graphics/shader/pixel/pixel_shader.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct PixelShaderCache
   {
   private:
      PixelShader* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;

   public:
      bool init(LinearAllocator& a, u32 maxSlots)
      {
         CORE_INIT_START(PixelShaderCache);

         m_buffer = allocate<PixelShader>(a, maxSlots);
         m_maxSlots = maxSlots;
         m_usedSlots = 0;

         CORE_INIT_END;
      }

      HPixelShader insert(PixelShader ps)
      {
         HPixelShader result{};

         for( u16 i = 0; i < m_maxSlots; ++i )
         {
            if( m_buffer[i].unloaded() )
            {
               m_buffer[i] = ps;
               result.init(i);
               ++m_usedSlots;
            }
         }

         return result;
      }

      PixelShader remove(HPixelShader handle)
      {
         PixelShader ps = m_buffer[handle.getIndex()];
         --m_usedSlots;
         return ps;
      }

      PixelShader getData(HPixelShader handle)
      {
         PixelShader result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_usedSlots;
      }

   };
}
