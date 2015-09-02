#pragma once
/********************************************
*  contents:   resource handle type, used with Cache
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/utility.h"
/******* end header inclusion *******/

namespace core
{
#define CORE_RESOURCE_HANDLE(name) struct tag_##name; typedef ResourceHandle<tag_##name> name

   template<typename TAG>
   struct ResourceHandle
   {
   public:
      ResourceHandle() : m_handle(0)
      {
      }

      void init(u16 index)
      {
         CORE_ASSERT_DEBUG(AssertLevel::Fatal, isNull(), "The handler is not null and init was called!");
         CORE_ASSERT_DEBUG(AssertLevel::Fatal, index <= MAX_INDEX, "The index is too large!");

         core_local_persist u16 s_autoMagic = 0;
         if( ++s_autoMagic > MAX_MAGIC )
         {
            s_autoMagic = 1; //0 is used for null handle
         }
         m_index = index;
         m_magic = s_autoMagic;
      }

      u16 getIndex() const
      {
         return m_index;
      }
      u16 getMagic() const
      {
         return m_magic;
      }
      u32 getHandle() const
      {
         return m_handle;
      }
      bool isNull() const
      {
         return !m_handle;
      }
      operator u32() const
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
            u16 m_index;
            u16 m_magic;
         };
         u32 m_handle;
      };
   };

   template<typename TAG>
   inline bool operator==(ResourceHandle<TAG> l, ResourceHandle<TAG> r)
   {
      return l.getHandle() == r.getHandle();
   }
   template<typename TAG>
   inline bool operator!=(ResourceHandle<TAG> l, ResourceHandle<TAG> r)
   {
      return !(l == r);
   }
   template<typename TAG>
   inline bool operator<(ResourceHandle<TAG> l, ResourceHandle<TAG> r)
   {
      return (l.getHandle() < r.getHandle());
   }
}
