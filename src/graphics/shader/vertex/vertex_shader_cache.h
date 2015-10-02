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
      VertexShader* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;

   public:
      void init(Memory& m, u32 maxSlots)
      {
         m_buffer = emplaceArray<VertexShader>(m, maxSlots);
         CORE_ASSERT_DBGERR(m_buffer != nullptr, "Not enough memory to emplace VertexShaderCache storage.");
         m_maxSlots = maxSlots;
         m_usedSlots = 0;
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
