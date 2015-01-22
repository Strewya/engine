//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/BonusDatabase.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
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

      assert(bonusDb.size() == (uint32_t)BonusTypeCount);

      std::sort(bonusDb.begin(), bonusDb.end(),
                [](const Bonus& l, const Bonus& r)
      {
         return l.type < r.type;
      });
      BonusType type;

      
      type = Heal;
      bonusDb[type].name = "Heal";
      bonusDb[type].acquireLogic = [](Vec2 position, Player& picker, RainbowlandGame& game)
      {
         picker.health += 20;
         clamp<int32_t>(0, picker.maxHealth, picker.health);
      };
      bonusDb[type].timeoutLogic = [](RainbowlandGame& game) {}; //none



      type = MassHeal;
      bonusDb[type].name = "MassHeal";
      bonusDb[type].acquireLogic = [](Vec2 position, Player& picker, RainbowlandGame& game)
      {
         for( auto& player : game.m_players )
         {
            player.health += 20;
            clamp<int32_t>(0, player.maxHealth, player.health);
         }
      };
      bonusDb[type].timeoutLogic = [](RainbowlandGame& game) {}; //none



      type = AtomicBomb;
      bonusDb[type].name = "Atomic Bomb";
      bonusDb[type].acquireLogic = [](Vec2 position, Player& picker, RainbowlandGame& game)
      {
         generateBlast(game.m_blasts, position, 0, 12, 0.8f, 10, nullptr);
      };
      bonusDb[type].timeoutLogic = [](RainbowlandGame& game) {}; //none

      
      
      type = IncreasedRateOfFire;
      bonusDb[type].name = "Shooter";
      bonusDb[type].durationMicros = nSeconds::toMicros(15U);
      bonusDb[type].acquireLogic = [=](Vec2 position, Player& picker, RainbowlandGame& game)
      {
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
      };
      bonusDb[type].timeoutLogic = [=](RainbowlandGame& game)
      {
         for( auto& player : game.m_players )
         {
            player.rateOfFireMultiplier *= 2;
            player.reloadMultiplier *= 2;
            calculateWeaponBonuses(player, game.m_weaponDatabase);
         }
         auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
         game.m_activeBonuses[index] = game.m_activeBonuses.back();
         game.m_activeBonuses.pop_back();
      };

      
      
      type = IncreasedMovementSpeed;
      bonusDb[type].name = "Runner";
      bonusDb[type].durationMicros = nSeconds::toMicros(15U);
      bonusDb[type].acquireLogic = [=](Vec2 position, Player& picker, RainbowlandGame& game)
      {
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
      };
      bonusDb[type].timeoutLogic = [=](RainbowlandGame& game)
      {
         for( auto& player : game.m_players )
         {
            player.maxSpeed /= 1.4f;
         }
         auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
         game.m_activeBonuses[index] = game.m_activeBonuses.back();
         game.m_activeBonuses.pop_back();
      };

     
      
      type = SlowTime;
      bonusDb[type].name = "Time lords";
      bonusDb[type].durationMicros = nSeconds::toMicros(8U);
      bonusDb[type].acquireLogic = [=](Vec2 position, Player& picker, RainbowlandGame& game)
      {
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
      };
      bonusDb[type].timeoutLogic = [=](RainbowlandGame& game)
      {
         game.m_currentTimeScale *= 2.5;// 1 div 0.4 == 2.5
         game.m_gameplayTimer.setTimeScale(game.m_currentTimeScale);
         auto index = filterFind(game.m_activeBonuses, [=](const ActiveBonus& e) { return type == e.type; });
         game.m_activeBonuses[index] = game.m_activeBonuses.back();
         game.m_activeBonuses.pop_back();
      };
   }
}