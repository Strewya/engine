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
   private:
      Allocator* m_allocator;
      VertexShader* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;

   public:
      void init(Allocator& a, u32 maxSlots)
      {
         m_allocator = &a;
         m_buffer = m_allocator->allocateArray<VertexShader>(maxSlots);
         m_maxSlots = maxSlots;
         m_usedSlots = 0;
      }
      void shutdown()
      {
         m_allocator->deallocateArray(m_buffer);
      }

      HVertexShader insert(VertexShader vs)
      {
         HVertexShader result{};

         if( m_maxSlots != m_usedSlots )
         {
            for( u16 i = 0; i < m_maxSlots; ++i )
            {
               if( m_buffer[i].unloaded() )
               {
                  m_buffer[i] = vs;
                  result.init(i);
                  ++m_usedSlots;
                  break;
               }
            }
         }

         return result;
      }

      VertexShader remove(HVertexShader handle)
      {
         VertexShader vs = m_buffer[handle.getIndex()];
         m_buffer[handle.getIndex()] = {};
         --m_usedSlots;
         return vs;
      }

      VertexShader getData(HVertexShader handle)
      {
         VertexShader result = m_buffer[handle.getIndex()];
         return result;
      }

      u32 getCount() const
      {
         return m_usedSlots;
      }
   };
}
