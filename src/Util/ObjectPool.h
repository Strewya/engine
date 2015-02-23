#pragma once
/********************************************
*	class:	ObjectPool
*	usage:
********************************************/
/******* C++ headers *******/
#include <algorithm>
#include <stdint.h>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end header inclusion *******/

namespace Core
{
   template<typename T> class ObjectPool
   {
   public:
      ObjectPool(uint32_t initialSize = 4);

      uint32_t create();
      T* get(uint32_t id);
      std::vector<uint32_t> getActiveIDs() const;

      void swapData(uint32_t id1, uint32_t id2);
      void release(uint32_t id);
      void clear();

      void setSize(uint32_t size);

   private:
      std::vector<T> m_data;
      std::vector<uint32_t> m_allocated;
      uint32_t m_firstFree;
   };

   template<typename T> ObjectPool<T>::ObjectPool(uint32_t initialSize)
      : m_firstFree(0)
   {
      setSize(initialSize);
   }

   template<typename T> uint32_t ObjectPool<T>::create()
   {
      if( m_firstFree == m_allocated.size() )
      {
         setSize(m_data.size() * 2);
      }
      return m_allocated[m_firstFree++];
   }

   template<typename T> std::vector<uint32_t> ObjectPool<T>::getActiveIDs() const
   {
      std::vector<uint32_t> ret(m_allocated.begin(), m_allocated.begin() + m_firstFree);
      std::sort(ret.begin(), ret.end());
      return ret;
   }

   template<typename T> T* ObjectPool<T>::get(uint32_t id)
   {
      T* ret = nullptr;
      if( id > 0 && id <= m_data.size() )
      {
         ret = &m_data[id - 1];
      }
      return ret;
   }

   template<typename T> void ObjectPool<T>::swapData(uint32_t id1, uint32_t id2)
   {
      --id1; --id2;
      if( id1 < m_data.size() && id2 < m_data.size() )
      {
         std::swap(m_data[id1], m_data[id2]);
      }
   }

   template<typename T> void ObjectPool<T>::release(uint32_t id)
   {
      auto index = valueFind(m_allocated, id);
      if( index < m_firstFree )
      {
         --m_firstFree;
         if( index != m_firstFree )
         {
            std::swap(m_allocated[index], m_allocated[m_firstFree]);
         }
      }
   }

   template<typename T> void ObjectPool<T>::clear()
   {
      m_firstFree = 0;
   }

   template<typename T> void ObjectPool<T>::setSize(uint32_t size)
   {
      if( size > m_data.size() )
      {
         m_data.resize(size);
         uint32_t i = m_allocated.size();
         uint32_t addCount = size - i;
         std::generate_n(std::back_inserter(m_allocated), addCount, [&]() { return ++i; });
      }
   }
}
