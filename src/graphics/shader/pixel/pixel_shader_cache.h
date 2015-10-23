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
      void init(Memory& m, u32 maxSlots)
      {
         m_buffer = emplaceArray<PixelShader>(m, maxSlots);
         CORE_ASSERT_DBGERR(m_buffer != nullptr, "Not enough memory to emplace PixelShaderCache storage.");
         m_maxSlots = maxSlots;
         m_usedSlots = 0;
      }

      HPixelShader insert(PixelShader ps)
      {
         HPixelShader result{};

         if( m_maxSlots != m_usedSlots )
         {
            for( u16 i = 0; i < m_maxSlots; ++i )
            {
               if( m_buffer[i].unloaded() )
               {
                  m_buffer[i] = ps;
                  result.init(i);
                  ++m_usedSlots;
                  break;
               }
            }
         }

         return result;
      }

      PixelShader remove(HPixelShader handle)
      {
         PixelShader ps = m_buffer[handle.getIndex()];
         m_buffer[handle.getIndex()] = {};
         --m_usedSlots;
         return ps;
      }

      PixelShader getData(HPixelShader handle) const
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
