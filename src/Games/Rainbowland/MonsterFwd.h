#pragma once
/********************************************
*  contents:   Monster related forward declarations
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   enum BrainState : int;
   enum MonsterType : int;

   struct Brain;
   struct Monster;
   struct MonsterSpawner;

   typedef std::vector<Monster> VMonsters;
   typedef std::vector<MonsterSpawner> VMonsterSpawners;
}
