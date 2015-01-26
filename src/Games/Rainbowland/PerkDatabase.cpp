//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/PerkDatabase.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/Circle.h>
#include <Util/CollisionChecks.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   void initPerkDatabase(VPerks& perkDb)
   {
      perkDb =
      {
         {PerkType::Regeneration},
         {PerkType::GreaterRegeneration},
         {PerkType::PoisonBullets},
         {PerkType::Radioactive},
         {PerkType::AmmoManiac},
         {PerkType::InstantWinner},
         {PerkType::Fastloader},
         {PerkType::ReflexBoosted},
         {PerkType::Dodger},
         {PerkType::Ninja},
         {PerkType::Assassin},
         {PerkType::HealthBoost},
         {PerkType::EquipedWeaponDamageBoost},
         {PerkType::EquipedWeaponAmmoBoost},
         {PerkType::FasterCooldown},
         {PerkType::HotTempered},
         {PerkType::AngryReloader},
         {PerkType::StationaryReloader},
         {PerkType::FastShot},
         {PerkType::ExtraBullet},
         {PerkType::PerkExpert},
         {PerkType::PerkMaster},
         {PerkType::MonsterCleaner}
      };

      assert(perkDb.size() == (uint32_t)PerkTypeCount && "Perk database not initialized properly");

      std::sort(perkDb.begin(), perkDb.end(),
                [](const Perk& l, const Perk& r)
      {
         return l.type < r.type;
      });

      CooldownTimer cdt;
      PeriodicTimer pt;
      PerkType type;

      type = Regeneration;
      perkDb[type].name = "Regeneration";
      perkDb[type].description = "Slowly gives back some health";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.regenDelayForOneHealth = 4;
      };
      perkDb[type].updateLogic = [=](Player& player, RainbowlandGame& game) mutable
      {
         auto& timer = pt;
         timer.setPeriodMicros(secondsToMicros(player.regenDelayForOneHealth));
         timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
         if( timer.hasElapsed() )
         {
            timer.period();
            player.health += 1;
            clamp<int32_t>(0, player.maxHealth, player.health);
         }
      };



      type = GreaterRegeneration;
      perkDb[type].name = "Greater Regeneration";
      perkDb[type].description = "Gives back some health a little bit faster";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         auto hasReqs = filterFind(player.acquiredPerks, [](PerkType perk)
         {
            return perk == Regeneration;
         }) != player.acquiredPerks.size();
         return hasReqs;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.regenDelayForOneHealth = 2;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = PoisonBullets;
      perkDb[type].name = "Heavy metal bullets";
      perkDb[type].description = "Metal bullets for hardcore damage";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.bonusDamage += 10;
         calculateWeaponBonuses(player, game.m_weaponDatabase);
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = Radioactive;
      perkDb[type].name = "Radioactive";
      perkDb[type].description = "You become a child of Chernobyl, radiating gamma rays";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game) {};
      perkDb[type].updateLogic = [=](Player& player, RainbowlandGame& game) mutable
      {
         auto& timer = pt;
         timer.setPeriodMicros(secondsToMicros(0.5f));
         timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
         if( timer.hasElapsed() )
         {
            timer.period();
            Circle radioactiveArea{player.transform.position, 5};
            for( auto& monsta : game.m_monsters )
            {
               auto bCollider = monsta.collisionData_hitbox;
               bCollider.center = monsta.transform.position;
               if( isCircleTouchingCircle(bCollider, radioactiveArea) )
               {
                  monsta.health -= 1;
               }
            }
         }
      };



      type = AmmoManiac;
      perkDb[type].name = "Ammo Maniac";
      perkDb[type].description = "You stuff 30% more bullets into your weapon";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.ammoMultiplier = 1.3f;
         calculateWeaponBonuses(player, game.m_weaponDatabase);
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = InstantWinner;
      perkDb[type].name = "Instant Healer";
      perkDb[type].description = "Nurse Nina fix you.";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.health = player.maxHealth;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = Fastloader;
      perkDb[type].name = "Fastloader";
      perkDb[type].description = "Chuck Norris teaches you reload-fu, the mastery of reloading your gun really fast";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.reloadMultiplier *= 0.7f;
         calculateWeaponBonuses(player, game.m_weaponDatabase);
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = ReflexBoosted;
      perkDb[type].name = "Reflex Boosted";
      perkDb[type].description = "Time slows down a bit around you";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         float manip = 0.1f / game.m_playerCount;
         game.m_currentTimeScale -= manip;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = Dodger;
      perkDb[type].name = "Dodger";
      perkDb[type].description = "You get a 10% chance to dodge each attack. Handy?";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         player.dodgeChance = 10;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = Ninja;
      perkDb[type].name = "Ninja";
      perkDb[type].description = "Your dodge chance is increased to 15%";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         auto hasReqs = filterFind(player.acquiredPerks, [](PerkType perk)
         {
            return perk == Dodger;
         }) != player.acquiredPerks.size();
         return hasReqs;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         player.dodgeChance = 15;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = Assassin;
      perkDb[type].name = "Assassin";
      perkDb[type].description = "Your dodge chance is increased to 20%, reaching mastery";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         auto hasReqs = filterFind(player.acquiredPerks, [](PerkType perk)
         {
            return perk == Ninja;
         }) != player.acquiredPerks.size();
         return hasReqs;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         player.dodgeChance = 20;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = HealthBoost;
      perkDb[type].name = "HealthBoost";
      perkDb[type].description = "You can take more hits from monsters. About 3 hit points worth of hits.";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         player.maxHealth += 3;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = EquipedWeaponDamageBoost;
      perkDb[type].name = "Weapon damage";
      perkDb[type].description = "Your current weapon gets more damage, but changing it removes the bonus";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         player.currentWeapon.damage += 2;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = EquipedWeaponAmmoBoost;
      perkDb[type].name = "Weapon ammo";
      perkDb[type].description = "Your current weapon gets more bullets in the clip, but changing it removes the bonus";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         player.currentWeapon.maxAmmo += 1;

      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = FasterCooldown;
      perkDb[type].name = "Cooled skill";
      perkDb[type].description = "You know how to use your skill more often!";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         if( *player.skillCooldown <= 10 )
         {
            return false;
         }
         return true;
      };
      perkDb[type].acquireLogic = [=](Player& player, RainbowlandGame& game)
      {
         (*player.skillCooldown) -= 1;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = HotTempered;
      perkDb[type].name = "Hot tempered";
      perkDb[type].description = "You have anger management issues, so you discharge it in the form of bullets around you.";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game) {};
      perkDb[type].updateLogic = [=](Player& player, RainbowlandGame& game) mutable
      {
         auto& timer = pt;
         timer.setPeriodMicros(secondsToMicros(10U));
         timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
         if( timer.hasElapsed() )
         {
            timer.period();
            Vec2f directions[] =
            {
               Vec2f{-1, 1}, Vec2f{-0.5f, 1}, Vec2f{0, 1}, Vec2f{0.5f, 1}, Vec2f{1, 1},
               Vec2f{-1, 0.5f}, Vec2f{1, 0.5f},
               Vec2f{-1, 0}, Vec2f{1, 0},
               Vec2f{-1, -0.5f}, Vec2f{1, -0.5f},
               Vec2f{-1, -1}, Vec2f{-0.5f, -1}, Vec2f{0, -1}, Vec2f{0.5f, -1}, Vec2f{1, -1}
            };

            for( auto aim : directions )
            {
               generateBullets(game.m_bullets, game.m_randomGenerator, 1, 0, player.transform.position,
                               Vec2f::normalize(aim), 50, false, &player);
            }
         }
      };



      type = AngryReloader;
      perkDb[type].name = "Angry Reloader";
      perkDb[type].description = "You hate reloading so much you shoot rockets out of your ass while doing so.";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game) {};
      std::array<bool, 4> firedOnReload = {false, false, false, false};
      perkDb[type].updateLogic = [=](Player& player, RainbowlandGame& game) mutable
      {
         if( player.currentWeapon.ammo <= 0 )
         {
            if( !firedOnReload[player.id] )
            {
               firedOnReload[player.id] = true;
               generateRocket(game.m_rockets, player.transform.position,
                              Vec2f::normalize(player.transform.position - player.aim), 30, &player);
            }
         }
         else
         {
            firedOnReload[player.id] = false;
         }
      };



      type = StationaryReloader;
      perkDb[type].name = "Stationary Reloader";
      perkDb[type].description = "When you concentrate, you reload so much faster";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game) {};
      std::array<bool, 4> reloadIncreased = {false, false, false, false};
      perkDb[type].updateLogic = [=](Player& player, RainbowlandGame& game) mutable
      {
         if( player.currentSpeed < 0.01f && !reloadIncreased[player.id] )
         {
            player.reloadMultiplier *= 0.5f;
            calculateWeaponBonuses(player, game.m_weaponDatabase);
            reloadIncreased[player.id] = true;
         }
         if( player.currentSpeed >= 0.01f && reloadIncreased[player.id] )
         {
            player.reloadMultiplier *= 2;
            calculateWeaponBonuses(player, game.m_weaponDatabase);
            reloadIncreased[player.id] = false;
         }
      };



      type = FastShot;
      perkDb[type].name = "John Wayne";
      perkDb[type].description = "You really know how to shoot faster than the other guy.";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.rateOfFireMultiplier *= 0.8f;
         calculateWeaponBonuses(player, game.m_weaponDatabase);
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = ExtraBullet;
      perkDb[type].name = "Bulleteer";
      perkDb[type].description = "You're a magician with guns, firing an extra bullet on every shot";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         player.extraBullets = 1;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = PerkExpert;
      perkDb[type].name = "Perk Expert";
      perkDb[type].description = "Congratulations, you found an extra perk to choose from on levelup!";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         ++player.perksPerLevel;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = PerkMaster;
      perkDb[type].name = "Perk Master";
      perkDb[type].description = "Congratulations, you found an extra perk to choose from on levelup!";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         auto hasReqs = filterFind(player.acquiredPerks, [](PerkType perk)
         {
            return perk == PerkExpert;
         }) != player.acquiredPerks.size();
         return hasReqs;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         ++player.perksPerLevel;
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};



      type = MonsterCleaner;
      perkDb[type].name = "Jackson's Deal";
      perkDb[type].description = "Trade 1/3 of your maximum health to kill every monster, without gaining experience. You might die!";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = [](Player& player)
      {
         return true;
      };
      perkDb[type].acquireLogic = [](Player& player, RainbowlandGame& game)
      {
         auto third = player.maxHealth / 3;
         player.maxHealth -= third;
         player.health -= third;
         for( auto& monsta : game.m_monsters )
         {
            monsta.health = 0;
         }
         VKillLocations kills;
         killMonsters(game, kills, false);
         generateSplatter(kills, game);
      };
      perkDb[type].updateLogic = [](Player& player, RainbowlandGame& game) {};
   }
}
