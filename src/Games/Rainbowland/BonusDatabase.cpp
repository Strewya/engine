//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/BonusDatabase.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Bonus.h>
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
#define ACQUIRE(name) acquire_##name
#define TIMEOUT(name) timeout_##name

#define ACQUIRE_FN(name) BONUS_ACQUIRE_FN(ACQUIRE(name))
#define TIMEOUT_FN(name) BONUS_TIMEOUT_FN(TIMEOUT(name))

#define DECLARE_LOGIC_FNS(name) \
   static ACQUIRE_FN(name); \
   static TIMEOUT_FN(name)


   DECLARE_LOGIC_FNS(AtomicBomb);
   DECLARE_LOGIC_FNS(Heal);
   DECLARE_LOGIC_FNS(IncreasedMovementSpeed);
   DECLARE_LOGIC_FNS(IncreasedRateOfFire);
   DECLARE_LOGIC_FNS(MassHeal);
   DECLARE_LOGIC_FNS(SlowTime);

   void initBonusDatabase(VBonuses& bonusDb)
   {
      bonusDb =
      {
         {BonusType::AtomicBomb},
         {BonusType::Heal},
         {BonusType::IncreasedMovementSpeed},
         {BonusType::IncreasedRateOfFire},
         {BonusType::MassHeal},
         {BonusType::SlowTime}
      };

      assert(bonusDb.size() == (uint32_t)BonusTypeCount && "Bonus database not initialized properly");

      std::sort(bonusDb.begin(), bonusDb.end(),
                [](const Bonus& l, const Bonus& r)
      {
         return l.type < r.type;
      });
      BonusType type;

      type = Heal;
      bonusDb[type].name = "Heal";
      bonusDb[type].durationMicros = 0;
      bonusDb[type].acquireLogic = ACQUIRE(Heal);
      bonusDb[type].timeoutLogic = nullptr;

      type = MassHeal;
      bonusDb[type].name = "MassHeal";
      bonusDb[type].durationMicros = 0;
      bonusDb[type].acquireLogic = ACQUIRE(MassHeal);
      bonusDb[type].timeoutLogic = nullptr;

      type = AtomicBomb;
      bonusDb[type].name = "Atomic Bomb";
      bonusDb[type].durationMicros = 0;
      bonusDb[type].acquireLogic = ACQUIRE(AtomicBomb);
      bonusDb[type].timeoutLogic = nullptr;

      type = IncreasedRateOfFire;
      bonusDb[type].name = "Shooter";
      bonusDb[type].durationMicros = secondsToMicros(15U);
      bonusDb[type].acquireLogic = ACQUIRE(IncreasedRateOfFire);
      bonusDb[type].timeoutLogic = TIMEOUT(IncreasedRateOfFire);

      type = IncreasedMovementSpeed;
      bonusDb[type].name = "Runner";
      bonusDb[type].durationMicros = secondsToMicros(15U);
      bonusDb[type].acquireLogic = ACQUIRE(IncreasedMovementSpeed);
      bonusDb[type].timeoutLogic = TIMEOUT(IncreasedMovementSpeed);

      type = SlowTime;
      bonusDb[type].name = "Time lords";
      bonusDb[type].durationMicros = secondsToMicros(8U);
      bonusDb[type].acquireLogic = ACQUIRE(SlowTime);
      bonusDb[type].timeoutLogic = TIMEOUT(SlowTime);
   }

   ACQUIRE_FN(AtomicBomb)
   {
      generateBlast(game.m_blasts, position, 0, 12, 0.8f, 10, nullptr);
   }
   
   ACQUIRE_FN(Heal)
   {
      player.health += 20;
      clamp<int32_t>(0, player.maxHealth, player.health);
   };

   ACQUIRE_FN(IncreasedMovementSpeed)
   {
      auto type = IncreasedMovementSpeed;
      auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
      if( index == game.m_activeBonuses.size() )
      {
         game.m_activeBonuses.emplace_back();
         game.m_activeBonuses[index].timer.setDurationMicros(game.m_bonusDatabase[type].durationMicros);
         game.m_activeBonuses[index].type = type;
         for( auto& player : game.m_players )
         {
            player.maxSpeed *= 1.4f;
         }
      }
      game.m_activeBonuses[index].timer.reset();
   }

   TIMEOUT_FN(IncreasedMovementSpeed)
   {
      auto type = IncreasedMovementSpeed;
      for( auto& player : game.m_players )
      {
         player.maxSpeed /= 1.4f;
      }
      auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
      game.m_activeBonuses[index] = game.m_activeBonuses.back();
      game.m_activeBonuses.pop_back();
   }

   ACQUIRE_FN(IncreasedRateOfFire)
   {
      auto type = IncreasedRateOfFire;
      auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
      if( index == game.m_activeBonuses.size() )
      {
         game.m_activeBonuses.emplace_back();
         game.m_activeBonuses[index].timer.setDurationMicros(game.m_bonusDatabase[type].durationMicros);
         game.m_activeBonuses[index].type = type;
         for( auto& player : game.m_players )
         {
            player.rateOfFireMultiplier *= 0.5f;
            player.reloadMultiplier *= 0.5f;
            calculateWeaponBonuses(player, game.m_weaponDatabase);
         }
      }
      game.m_activeBonuses[index].timer.reset();
   }

   TIMEOUT_FN(IncreasedRateOfFire)
   {
      auto type = IncreasedRateOfFire;
      for( auto& player : game.m_players )
      {
         player.rateOfFireMultiplier *= 2;
         player.reloadMultiplier *= 2;
         calculateWeaponBonuses(player, game.m_weaponDatabase);
      }
      auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
      game.m_activeBonuses[index] = game.m_activeBonuses.back();
      game.m_activeBonuses.pop_back();
   }

   ACQUIRE_FN(MassHeal)
   {
      for( auto& player : game.m_players )
      {
         player.health += 20;
         clamp<int32_t>(0, player.maxHealth, player.health);
      }
   }
   

   ACQUIRE_FN(SlowTime)
   {
      auto type = SlowTime;
      auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
      if( index == game.m_activeBonuses.size() )
      {
         game.m_activeBonuses.emplace_back();
         game.m_activeBonuses[index].timer.setDurationMicros(game.m_bonusDatabase[type].durationMicros);
         game.m_activeBonuses[index].type = type;
         game.m_currentTimeScale *= 0.4;
         game.m_gameplayTimer.setTimeScale(game.m_currentTimeScale);
      }
      game.m_activeBonuses[index].timer.reset();
   }

   TIMEOUT_FN(SlowTime)
   {
      auto type = SlowTime;
      game.m_currentTimeScale *= 2.5;// 1 div 0.4 == 2.5
      game.m_gameplayTimer.setTimeScale(game.m_currentTimeScale);
      auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
      game.m_activeBonuses[index] = game.m_activeBonuses.back();
      game.m_activeBonuses.pop_back();
   }

}