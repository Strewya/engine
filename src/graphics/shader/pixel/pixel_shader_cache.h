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
      bool init(LinearAllocator& a, u32 maxSlots)
      {
         CORE_INIT_START(PixelShaderCache);

         m_count = 0;
         m_maxSlots = maxSlots;
         m_buffer = allocate<PixelShader>(a, maxSlots);
         m_freelist.init(m_buffer, maxSlots);

         CORE_INIT_END;
      }

      HPixelShader insert(PixelShader vs)
      {
         HPixelShader result{};
         auto* slot = allocate(m_freelist);
         if( slot )
         {
            PixelShader* shaderSlot = (PixelShader*)slot;
            *shaderSlot = vs;
            u16 index = u16(shaderSlot - m_buffer);
            ++m_count;
            result.init(index);
         }
         return result;
      }

      PixelShader remove(HPixelShader handle)
      {
         u16 index = handle.getIndex();
         PixelShader vs = m_buffer[index];
         deallocate(m_freelist, &m_buffer[index]);
         --m_count;
         return vs;
      }

      PixelShader getData(HPixelShader handle)
      {
         PixelShader result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_count;
      }

   private:
      PixelShader* m_buffer;
      Freelist m_freelist;
      u32 m_maxSlots;
      u32 m_count;
   };
}
