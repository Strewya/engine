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
   public:
      DATA& acquire(HANDLE& handle)
      {
         uint16_t index;
         if( m_freeSlots.empty() )
         {
            index = m_magicNumbers.size();
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
         uint16_t index = handle.getIndex();
         assert(index < m_data.size());
         assert(m_magicNumbers[index] == handle.getMagic());

         m_magicNumbers[index] = 0;
         m_freeSlots.push_back(index);
      }

      DATA* dereference(HANDLE handle)
      {
         if( handle.isNull() )
         {
            uint16_t index = handle.getIndex();
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

   private:
      typedef std::vector<DATA> DataVec;
      typedef std::vector<uint16_t> MagicVec;
      typedef std::vector<uint16_t> FreeVec;

      DataVec m_data;
      MagicVec m_magicNumbers;
      FreeVec m_freeSlots;
   };
}
