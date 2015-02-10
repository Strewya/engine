#pragma once
/********************************************
*  contents:   Bonus related forward declarations
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/PlayerFwd.h>
#include <Util/Vec2Fwd.h>
/******* end header inclusion *******/

namespace Core
{
   // TEMPORARY until i separate game state from the class containing it
   class RainbowlandGame;


   enum BonusType : int;
   
   struct Bonus;
   struct ActiveBonus;

   typedef std::vector<Bonus> VBonuses;
   typedef std::vector<ActiveBonus> VActiveBonuses;

#define BONUS_ACQUIRE_FN(name) void name(Vec2f position, Player& player, RainbowlandGame& game)
#define BONUS_TIMEOUT_FN(name) void name(RainbowlandGame& game)

   typedef BONUS_ACQUIRE_FN(AcquireBonus);
   typedef BONUS_TIMEOUT_FN(TimeoutBonus);
}

