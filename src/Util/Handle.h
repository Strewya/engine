#pragma once
/********************************************
*  contents:   Handle
*  usage:
********************************************/
/******* C++ headers *******/
#include <cassert>
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   template<typename TAG>
   class Handle
   {
   public:
      Handle() : m_handle(0)
      {
      }

      void init(uint16_t index)
      {
         assert(isNull());
         assert(index <= MAX_INDEX);

         static uint16_t s_autoMagic = 0;
         if( ++s_autoMagic > MAX_MAGIC )
         {
            s_autoMagic = 1; //0 is used for null handle
         }
         m_index = index;
         m_magic = s_autoMagic;
      }

      uint16_t getIndex() const
      {
         return m_index;
      }
      uint16_t getMagic() const
      {
         return m_magic;
      }
      uint32_t getHandle() const
      {
         return m_handle;
      }
      bool isNull() const
      {
         return !m_handle;
      }
      operator uint32_t() const
      {
         return m_handle;
      }

   private:
      enum
      {
         MAX_BITS_INDEX = 16,
         MAX_BITS_MAGIC = 16,
         MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
         MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1
      };
      union
      {
         struct
         {
            uint16_t m_index;
            uint16_t m_magic;
         };
         uint32_t m_handle;
      };
   };

   template<typename TAG>
   inline bool operator==(Handle<TAG> l, Handle<TAG> r)
   {
      return l.getHandle() == r.getHandle();
   }
   template<typename TAG>
   inline bool operator!=(Handle<TAG> l, Handle<TAG> r)
   {
      return !(l == r);
   }
}
