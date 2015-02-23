//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Monster.h>
/******* C++ headers *******/
#include <algorithm>
#include <iterator>
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   void MonsterObjectPool::setMaxSize(uint32_t maxSize)
   {
      monsters.resize(maxSize);
      std::generate_n(std::back_inserter(freeSlots), maxSize, [&]() { return --maxSize; });
   }

   Monster* MonsterObjectPool::newMonster()
   {
      if( freeSlots.empty() )
      {
         return nullptr;
      }
      auto slot = freeSlots.back();
      freeSlots.pop_back();
      return &monsters[slot];
   }

   void MonsterObjectPool::releaseMonster(Monster* m)
   {
      for( uint32_t i = 0; i < monsters.size(); ++i )
      {
         if( &monsters[i] == m )
         {
            freeSlots.push_back(i);
            std::sort(freeSlots.begin(), freeSlots.end(), [](uint32_t l, uint32_t r) { return l > r; });
         }
      }
   }

   void MonsterObjectPool::releaseAll()
   {
      uint32_t maxSize = monsters.size();
      freeSlots.clear();
      std::generate_n(std::back_inserter(freeSlots), maxSize, [&]() { return --maxSize; });
   }
}
