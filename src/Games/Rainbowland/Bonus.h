#pragma once
/********************************************
*  contents:   Bonus related definitions
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
#include <Games/Rainbowland/BonusFwd.h>
/******* extra headers *******/
#include <Util/Time.h>
/******* end header inclusion *******/

namespace Core
{
   enum BonusType : int
   {
      AtomicBomb,
      SlowTime,
      Heal,
      MassHeal,
      IncreasedMovementSpeed,
      IncreasedRateOfFire,
      BonusTypeCount
   };

   struct Bonus
   {
      BonusType type;
      std::string name;
      uint32_t durationMicros;
      AcquireBonus* acquireLogic;
      TimeoutBonus* timeoutLogic;
   };

   struct ActiveBonus
   {
      CooldownTimer timer;
      BonusType type;
   };
}
