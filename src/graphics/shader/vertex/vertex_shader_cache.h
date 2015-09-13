#pragma once
/********************************************
*  contents:   vertex shader resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/shader/shader_handle.h"
#include "graphics/shader/vertex/vertex_shader.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct VertexShaderCache
   {
      bool init(LinearAllocator& a, u32 maxSlots)
      {
         CORE_INIT_START(VertexShaderCache);

         m_count = 0;
         m_maxSlots = maxSlots;
         m_buffer = allocate<VertexShader>(a, maxSlots);
         m_freelist.init(m_buffer, maxSlots);

         CORE_INIT_END;
      }

      HVertexShader insert(VertexShader vs)
      {
         HVertexShader result{};
         u8* slot = allocate(m_freelist);
         if( slot )
         {
            VertexShader* shaderSlot = (VertexShader*)slot;
            *shaderSlot = vs;
            u16 index = u16(shaderSlot - m_buffer);
            ++m_count;
            result.init(index);
         }
         return result;
      }

      VertexShader remove(HVertexShader handle)
      {
         u16 index = handle.getIndex();
         VertexShader vs = m_buffer[index];
         deallocate(m_freelist, &m_buffer[index]);
         --m_count;
         return vs;
      }

      VertexShader getData(HVertexShader handle)
      {
         VertexShader result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_count;
      }

   private:
      VertexShader* m_buffer;
      Freelist m_freelist;
      u32 m_maxSlots;
      u32 m_count;
   };
}
