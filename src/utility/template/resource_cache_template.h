#pragma once
/********************************************
*  contents:   template class and iterator for any type of resource object
*  usage:
********************************************/
/******* c++ headers *******/
#include <vector>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/utility.h"
/******* end header inclusion *******/

namespace core
{
   template<typename CACHE>
   struct core_iterator
   {
   public:
      typedef typename CACHE::DataType DataType;

      DataType& operator*()
      {
         return m_cache->m_data[m_index];
      }
      DataType* operator->()
      {
         return &(m_cache->m_data[m_index]);
      }
      core_iterator& operator++()
      {
         u32 size = (u32)m_cache->m_magicNumbers.size();
         if( m_index < size )
         {
            while( (++m_index) < size && m_cache->m_magicNumbers[m_index] == 0 );
         }
         return *this;
      }
      core_iterator operator++(int)
      {
         auto self = *this;
         operator++();
         return self;
      }
      bool operator==(const core_iterator& other)
      {
         return m_cache == other.m_cache && m_index == other.m_index;
      }
      bool operator!=(const core_iterator& other)
      {
         return !(*this == other);
      }

   private:
      friend CACHE;
      CACHE* m_cache;
      u32 m_index;
   };

   template<typename DATA, typename HANDLE>
   struct Cache
   {
   public:
      typedef DATA DataType;
      typedef HANDLE HandleType;
      typedef Cache<DATA, HANDLE> ThisType;


      typedef std::vector<DATA> DataVec;
      typedef std::vector<u16> MagicVec;
      typedef std::vector<u16> FreeVec;
      typedef core_iterator<ThisType> iterator;

      DATA& acquire(HANDLE& handle)
      {
         u16 index;
         if( m_freeSlots.empty() )
         {
            index = (u16)m_magicNumbers.size();
            handle.init(index);
            m_data.emplace_back();
            m_magicNumbers.push_back(handle.getMagic());
         }
         else
         {
            index = m_freeSlots.back();
            handle.init(index);
            m_freeSlots.pop_back();
            m_magicNumbers[index] = handle.getMagic();
         }
         return m_data[index];
      }

      void release(HANDLE handle)
      {
         auto index = handle.getIndex();
         CORE_ASSERT_FATAL_DEBUG(index < m_data.size(), "Index is not valid!");
         CORE_ASSERT_FATAL_DEBUG(m_magicNumbers[index] == handle.getMagic(), "The magic number is not equal!");

         m_magicNumbers[index] = 0;
         m_freeSlots.push_back(index);
      }

      DATA* dereference(HANDLE handle)
      {
         if( !handle.isNull() )
         {
            auto index = handle.getIndex();
            if( index < m_data.size() && m_magicNumbers[index] == handle.getMagic() )
            {
               return &m_data[index];
            }
         }
         return nullptr;
      }
      
      const DATA* dereference(HANDLE handle) const
      {
         return const_cast<ThisType*>(this)->dereference(handle);
      }

      u32 getUsedHandleCount() const
      {
         return (u32)(m_magicNumbers.size() - m_freeSlots.size());
      }
      
      bool hasUsedHandles() const
      {
         return !!getUsedHandleCount();
      }

      iterator begin()
      {
         iterator it;
         it.m_cache = this;
         it.m_index = 0;
         while( it.m_index < m_magicNumbers.size() && m_magicNumbers[it.m_index] == 0 )
         {
            ++it.m_index;
         }
         return it;
      }
      iterator end()
      {
         iterator it;
         it.m_cache = this;
         it.m_index = (u32)m_magicNumbers.size();
         return it;
      }

   private:
      friend struct core_iterator<ThisType>;
      DataVec m_data;
      MagicVec m_magicNumbers;
      FreeVec m_freeSlots;
   };

   
}
