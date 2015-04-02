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
   template<typename CACHE>
   class core_iterator
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
         uint32_t size = m_cache->m_magicNumbers.size();
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
      uint32_t m_index;
   };

   template<typename DATA, typename HANDLE>
   class Cache
   {
   public:
      typedef DATA DataType;
      typedef HANDLE HandleType;
      typedef Cache<DATA, HANDLE> ThisType;


      typedef std::vector<DATA> DataVec;
      typedef std::vector<uint16_t> MagicVec;
      typedef std::vector<uint16_t> FreeVec;
      typedef core_iterator<ThisType> iterator;

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

      iterator begin()
      {
         iterator it;
         it.m_cache = this;
         it.m_index = 0;
         while( m_magicNumbers[it.m_index] == 0 )
         {
            ++it.m_index;
         }
         return it;
      }
      iterator end()
      {
         iterator it;
         it.m_cache = this;
         it.m_index = m_magicNumbers.size();
         return it;
      }

   private:
      friend class core_iterator<ThisType>;
      DataVec m_data;
      MagicVec m_magicNumbers;
      FreeVec m_freeSlots;
   };

   
}
