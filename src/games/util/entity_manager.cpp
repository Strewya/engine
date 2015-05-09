//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/util/entity_manager.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/util/entity_handle.h"
/******* end headers *******/

namespace core
{
   EntityHandle EntityManager::create()
   {
      uint32_t idx;
      if( m_freeIndex.size() > 128 )
      {
         idx = m_freeIndex.front();
         m_freeIndex.pop_front();
      }
      else
      {
         m_generation.push_back(0);
         idx = m_generation.size() - 1;
         assert(idx < (1 << EntityHandle::INDEX_BITS));
      }
      EntityHandle result;
      result.id = (m_generation[idx] << EntityHandle::INDEX_BITS) | idx;
      
      return result;
   }
   
   bool EntityManager::alive(EntityHandle e)
   {
      auto result = m_generation[e.index()] == e.generation();
      return result;
   }

   void EntityManager::kill(EntityHandle e)
   {
      auto idx = e.index();
      ++m_generation[idx];
      m_freeIndex.push_back(idx);
   }
}