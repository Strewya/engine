#pragma once
/********************************************
*  contents:   Cache
*  usage:
********************************************/
/******* C++ headers *******/
#include <cassert>
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   template<typename DATA, typename HANDLE>
   class Cache
   {
      typedef std::vector<DATA> DataVec;
      typedef std::vector<uint16_t> MagicVec;
      typedef std::vector<uint16_t> FreeVec;

   public:
      DATA& acquire(HANDLE& handle)
      {
         uint16_t index;
         if( m_freeSlots.empty() )
         {
            index = (uint16_t)m_magicNumbers.size();
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
         assert(index < m_data.size());
         assert(m_magicNumbers[index] == handle.getMagic());

         m_magicNumbers[index] = 0;
         m_freeSlots.push_back(index);
      }

      DATA* dereference(HANDLE handle)
      {
         if( handle.isNull() )
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
         typedef Cache<DATA, HANDLE> ThisType;
         return const_cast<ThisType*>(this)->dereference(handle);
      }

      uint32_t getUsedHandleCount() const
      {
         return m_magicNumbers.size() - m_freeSlots.size();
      }
      
      bool hasUsedHandles() const
      {
         return !!getUsedHandleCount();
      }

      friend class iterator;
      class iterator
      {
      public:
         DATA& operator*()
         {
            return m_cache->m_data[m_index];
         }
         iterator& operator++()
         {
            uint32_t size = m_magicNumbers.size();
            if( m_index < size )
            {
               while( (++m_index) < size && m_magicNumbers[m_index] == 0 );
            }
            return *this;
         }
         iterator operator++(int)
         {
            auto self = *this;
            operator++();
            return self;
         }
         bool operator==(const iterator& other)
         {
            return m_cache == other.m_cache && m_index == other.m_index;
         }
         bool operator!=(const iterator& other)
         {
            return !(*this == other);
         }
      private:
         Cache* m_cache;
         uint32_t m_index;
      };

      iterator begin()
      {
         uint32_t index = 0;
         while( m_magicNumbers[index] == 0 )
         {
            ++index;
         }
         return iterator{this, index};
      }
      iterator end()
      {
         return iterator{this, m_magicNumbers.size()};
      }

   private:
      
      DataVec m_data;
      MagicVec m_magicNumbers;
      FreeVec m_freeSlots;
   };
}
