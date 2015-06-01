//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/util/entity_manager.h"
/******* c++ headers *******/
#include <cassert>
/******* extra headers *******/
#include "games/util/entity.h"
/******* end headers *******/

namespace core
{
   Entity EntityManager::create()
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
         assert(idx < (1 << Entity::INDEX_BITS));
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
}