//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/entity_manager.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
/*
   Entity EntityManager::create()
   {
      u32 idx;
      if( m_freeIndex.size() > 128 )
      {
         idx = m_freeIndex.front();
         m_freeIndex.pop_front();
      }
      else
      {
         m_generation.push_back(0);
         idx = m_generation.size() - 1;
         CORE_ASSERT_DEBUG(idx < (1 << Entity::INDEX_BITS));
      }
      Entity result;
      result.id = (m_generation[idx] << Entity::INDEX_BITS) | idx;
      
      return result;
   }
   
   bool EntityManager::alive(Entity e)
   {
      auto result = m_generation[e.index()] == e.generation();
      return result;
   }

   void EntityManager::kill(Entity e)
   {
      auto idx = e.index();
      ++m_generation[idx];
      m_freeIndex.push_back(idx);
   }
*/
}