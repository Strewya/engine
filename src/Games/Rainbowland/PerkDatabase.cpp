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
#define CHECK(name) check_##name
#define ACQUIRE(name) acquire_##name
#define UPDATE(name) update_##name

#define CHECK_FN(name) PERK_CHECK_FN(CHECK(name))
#define ACQUIRE_FN(name) PERK_ACQUIRE_FN(ACQUIRE(name))
#define UPDATE_FN(name) PERK_UPDATE_FN(UPDATE(name))

#define DECLARE_LOGIC_FNS(name) \
   static CHECK_FN(name); \
   static ACQUIRE_FN(name); \
   static UPDATE_FN(name)

   DECLARE_LOGIC_FNS(Regeneration);
   DECLARE_LOGIC_FNS(GreaterRegeneration);
   DECLARE_LOGIC_FNS(PoisonBullets);
   DECLARE_LOGIC_FNS(Radioactive);
   DECLARE_LOGIC_FNS(AmmoManiac);
   DECLARE_LOGIC_FNS(InstantWinner);
   DECLARE_LOGIC_FNS(Fastloader);
   DECLARE_LOGIC_FNS(ReflexBoosted);
   DECLARE_LOGIC_FNS(Dodger);
   DECLARE_LOGIC_FNS(Ninja);
   DECLARE_LOGIC_FNS(Assassin);
   DECLARE_LOGIC_FNS(HealthBoost);
   DECLARE_LOGIC_FNS(EquipedWeaponDamageBoost);
   DECLARE_LOGIC_FNS(EquipedWeaponAmmoBoost);
   DECLARE_LOGIC_FNS(FasterCooldown);
   DECLARE_LOGIC_FNS(HotTempered);
   DECLARE_LOGIC_FNS(AngryReloader);
   DECLARE_LOGIC_FNS(StationaryReloader);
   DECLARE_LOGIC_FNS(FastShot);
   DECLARE_LOGIC_FNS(ExtraBullet);
   DECLARE_LOGIC_FNS(PerkExpert);
   DECLARE_LOGIC_FNS(PerkMaster);
   DECLARE_LOGIC_FNS(MonsterCleaner);


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
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(Regeneration);
      perkDb[type].updateLogic = UPDATE(Regeneration);

      type = GreaterRegeneration;
      perkDb[type].name = "Greater Regeneration";
      perkDb[type].description = "Gives back some health a little bit faster";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = CHECK(GreaterRegeneration);
      perkDb[type].acquireLogic = ACQUIRE(GreaterRegeneration);
      perkDb[type].updateLogic = nullptr;

      type = PoisonBullets;
      perkDb[type].name = "Heavy metal bullets";
      perkDb[type].description = "Metal bullets for hardcore damage";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(PoisonBullets);
      perkDb[type].updateLogic = nullptr;

      type = Radioactive;
      perkDb[type].name = "Radioactive";
      perkDb[type].description = "You become a child of Chernobyl, radiating gamma rays";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = nullptr;
      perkDb[type].updateLogic = UPDATE(Radioactive);

      type = AmmoManiac;
      perkDb[type].name = "Ammo Maniac";
      perkDb[type].description = "You stuff 30% more bullets into your weapon";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(AmmoManiac);
      perkDb[type].updateLogic = nullptr;

      type = InstantWinner;
      perkDb[type].name = "Instant Healer";
      perkDb[type].description = "Nurse Nina fix you.";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(InstantWinner);
      perkDb[type].updateLogic = nullptr;

      type = Fastloader;
      perkDb[type].name = "Fastloader";
      perkDb[type].description = "Chuck Norris teaches you reload-fu, the mastery of reloading your gun really fast";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(Fastloader);
      perkDb[type].updateLogic = nullptr;

      type = ReflexBoosted;
      perkDb[type].name = "Reflex Boosted";
      perkDb[type].description = "Time slows down a bit around you";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(ReflexBoosted);
      perkDb[type].updateLogic = nullptr;

      type = Dodger;
      perkDb[type].name = "Dodger";
      perkDb[type].description = "You get a 10% chance to dodge each attack. Handy?";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(Dodger);
      perkDb[type].updateLogic = nullptr;

      type = Ninja;
      perkDb[type].name = "Ninja";
      perkDb[type].description = "Your dodge chance is increased to 15%";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = CHECK(Ninja);
      perkDb[type].acquireLogic = ACQUIRE(Ninja);
      perkDb[type].updateLogic = nullptr;

      type = Assassin;
      perkDb[type].name = "Assassin";
      perkDb[type].description = "Your dodge chance is increased to 20%, reaching mastery";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = CHECK(Assassin);
      perkDb[type].acquireLogic = ACQUIRE(Assassin);
      perkDb[type].updateLogic = nullptr;

      type = HealthBoost;
      perkDb[type].name = "HealthBoost";
      perkDb[type].description = "You can take more hits from monsters. About 3 hit points worth of hits.";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(HealthBoost);
      perkDb[type].updateLogic = nullptr;

      type = EquipedWeaponDamageBoost;
      perkDb[type].name = "Weapon damage";
      perkDb[type].description = "Your current weapon gets more damage, but changing it removes the bonus";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(EquipedWeaponDamageBoost);
      perkDb[type].updateLogic = nullptr;

      type = EquipedWeaponAmmoBoost;
      perkDb[type].name = "Weapon ammo";
      perkDb[type].description = "Your current weapon gets more bullets in the clip, but changing it removes the bonus";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(EquipedWeaponAmmoBoost);
      perkDb[type].updateLogic = nullptr;

      type = FasterCooldown;
      perkDb[type].name = "Cooled skill";
      perkDb[type].description = "You know how to use your skill more often!";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = CHECK(FasterCooldown);
      perkDb[type].acquireLogic = ACQUIRE(FasterCooldown);
      perkDb[type].updateLogic = nullptr;

      type = HotTempered;
      perkDb[type].name = "Hot tempered";
      perkDb[type].description = "You have anger management issues, so you discharge it in the form of bullets around you.";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = nullptr;
      perkDb[type].updateLogic = UPDATE(HotTempered);

      type = AngryReloader;
      perkDb[type].name = "Angry Reloader";
      perkDb[type].description = "You hate reloading so much you shoot rockets out of your ass while doing so.";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = nullptr;

      perkDb[type].updateLogic = UPDATE(AngryReloader);

      type = StationaryReloader;
      perkDb[type].name = "Stationary Reloader";
      perkDb[type].description = "When you concentrate, you reload so much faster";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = nullptr;
      perkDb[type].updateLogic = UPDATE(StationaryReloader);

      type = FastShot;
      perkDb[type].name = "John Wayne";
      perkDb[type].description = "You really know how to shoot faster than the other guy.";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(FastShot);
      perkDb[type].updateLogic = nullptr;

      type = ExtraBullet;
      perkDb[type].name = "Bulleteer";
      perkDb[type].description = "You make your own clips that hold 6 extra bullets.";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(ExtraBullet);
      perkDb[type].updateLogic = nullptr;

      type = PerkExpert;
      perkDb[type].name = "Perk Expert";
      perkDb[type].description = "Congratulations, you found an extra perk to choose from on levelup!";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(PerkExpert);
      perkDb[type].updateLogic = nullptr;

      type = PerkMaster;
      perkDb[type].name = "Perk Master";
      perkDb[type].description = "Congratulations, you found an extra perk to choose from on levelup!";
      perkDb[type].repeatable = false;
      perkDb[type].dependencyCheck = CHECK(PerkMaster);
      perkDb[type].acquireLogic = ACQUIRE(PerkExpert); //same logic as perk expert, so reuse
      perkDb[type].updateLogic = nullptr;

      type = MonsterCleaner;
      perkDb[type].name = "Jackson's Deal";
      perkDb[type].description = "Trade 1/3 of your maximum health to kill every monster, without gaining experience. You might die!";
      perkDb[type].repeatable = true;
      perkDb[type].dependencyCheck = nullptr;
      perkDb[type].acquireLogic = ACQUIRE(MonsterCleaner);
      perkDb[type].updateLogic = nullptr;
   }



   ACQUIRE_FN(Regeneration)
   {
      player.regenDelayForOneHealth = 4;
   }
   UPDATE_FN(Regeneration)
   {
      auto& timer = aperk.timer;
      timer.setPeriodMicros(secondsToMicros(player.regenDelayForOneHealth));
      timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( timer.hasElapsed() )
      {
         timer.period();
         player.health += 1;
         clamp<int32_t>(0, player.maxHealth, player.health);
      }
   }

   CHECK_FN(GreaterRegeneration)
   {
      auto hasReqs = filterFind(player.acquiredPerks, [](const ActivePerk& perk)
      {
         return perk.type == Regeneration;
      }) != player.acquiredPerks.size();
      return hasReqs;
   }
   ACQUIRE_FN(GreaterRegeneration)
   {
      player.regenDelayForOneHealth = 2;
   }

   ACQUIRE_FN(PoisonBullets)
   {
      player.bonusDamage += 10;
      calculateWeaponBonuses(player, game.m_weaponDatabase);
   }

   UPDATE_FN(Radioactive)
   {
      auto& timer = aperk.timer;
      timer.setPeriodMicros(secondsToMicros(0.5f));
      timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( timer.hasElapsed() )
      {
         timer.period();
         Circle radioactiveArea{player.transform.position, 5};
         for( auto* monsta : game.m_monsters )
         {
            auto bCollider = monsta->collisionData_hitbox;
            bCollider.center = monsta->transform.position;
            if( isCircleTouchingCircle(bCollider, radioactiveArea) )
            {
               monsta->health -= 1;
            }
         }
      }
   }

   ACQUIRE_FN(AmmoManiac)
   {
      player.ammoMultiplier = 1.3f;
      calculateWeaponBonuses(player, game.m_weaponDatabase);
   }

   ACQUIRE_FN(InstantWinner)
   {
      player.health = player.maxHealth;
   }

   ACQUIRE_FN(Fastloader)
   {
      player.reloadMultiplier *= 0.7f;
      calculateWeaponBonuses(player, game.m_weaponDatabase);
   }

   ACQUIRE_FN(ReflexBoosted)
   {
      float manip = 0.1f / game.m_playerCount;
      game.m_currentTimeScale -= manip;
   }

   ACQUIRE_FN(Dodger)
   {
      player.dodgeChance = 10;
   }

   CHECK_FN(Ninja)
   {
      auto hasReqs = filterFind(player.acquiredPerks, [](const ActivePerk& perk)
      {
         return perk.type == Dodger;
      }) != player.acquiredPerks.size();
      return hasReqs;
   }
   ACQUIRE_FN(Ninja)
   {
      player.dodgeChance = 15;
   }

   CHECK_FN(Assassin)
   {
      auto hasReqs = filterFind(player.acquiredPerks, [](const ActivePerk& perk)
      {
         return perk.type == Ninja;
      }) != player.acquiredPerks.size();
      return hasReqs;
   }
   ACQUIRE_FN(Assassin)
   {
      player.dodgeChance = 20;
   }

   ACQUIRE_FN(HealthBoost)
   {
      player.maxHealth += 3;
   }

   ACQUIRE_FN(EquipedWeaponDamageBoost)
   {
      player.currentWeapon.damage += 2;
   }

   ACQUIRE_FN(EquipedWeaponAmmoBoost)
   {
      player.currentWeapon.maxAmmo += 1;
   }

   CHECK_FN(FasterCooldown)
   {
      if( player.skillCooldown <= 10 )
      {
         return false;
      }
      return true;
   }
   ACQUIRE_FN(FasterCooldown)
   {
      player.skillCooldown -= 1;
      auto cooldownMicros = secondsToMicros(player.skillCooldown);
      switch( player.ability )
      {
         case AT_HealingCloud:
         {
            game.m_healingCloud.timer.setDurationMicros(cooldownMicros);
         } break;
         case AT_DefenseMatrix:
         {
            game.m_defenseMatrix.timer.setDurationMicros(cooldownMicros);
         } break;
         case AT_TimeCapsule:
         {
            game.m_timeCapsule.timer.setDurationMicros(cooldownMicros);
         } break;
         case AT_Turret:
         {
            game.m_turret.timer.setDurationMicros(cooldownMicros);
         } break;
      }
   }

   UPDATE_FN(HotTempered)
   {
      auto& timer = aperk.timer;
      timer.setPeriodMicros(secondsToMicros(10U));
      timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( timer.hasElapsed() )
      {
         timer.period();
         Vec2f directions[] =
         {
            Vec2f{-1, 1},  Vec2f{-0.5f, 1},  Vec2f{0, 1},   Vec2f{0.5f, 1},   Vec2f{1, 1},
            Vec2f{-1, 0.5f},                                                  Vec2f{1, 0.5f},
            Vec2f{-1, 0},                                                     Vec2f{1, 0},
            Vec2f{-1, -0.5f},                                                 Vec2f{1, -0.5f},
            Vec2f{-1, -1}, Vec2f{-0.5f, -1}, Vec2f{0, -1},  Vec2f{0.5f, -1},  Vec2f{1, -1}
         };

         for( auto aim : directions )
         {
            generateBullets(game.m_bullets, game.m_randomGenerator, 1, 0, player.transform.position,
                            Vec2f::normalize(aim), 50, false, &player);
         }
      }
   }

   UPDATE_FN(AngryReloader)
   {
      static bool firedOnReload[4] = {false, false, false, false};
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
   }

   UPDATE_FN(StationaryReloader)
   {
      static bool reloadIncreased[4] = {false, false, false, false};
      if( player.currentSpeed < 0.01f && !reloadIncreased[player.id] )
      {
         player.reloadMultiplier *= 0.3f;
         calculateWeaponBonuses(player, game.m_weaponDatabase);
         reloadIncreased[player.id] = true;
      }
      if( player.currentSpeed >= 0.01f && reloadIncreased[player.id] )
      {
         player.reloadMultiplier /= 0.3f;
         calculateWeaponBonuses(player, game.m_weaponDatabase);
         reloadIncreased[player.id] = false;
      }
   }

   ACQUIRE_FN(FastShot)
   {
      player.rateOfFireMultiplier *= 0.8f;
      calculateWeaponBonuses(player, game.m_weaponDatabase);
   }

   ACQUIRE_FN(ExtraBullet)
   {
      player.extraBullets = 6;
      calculateWeaponBonuses(player, game.m_weaponDatabase);
   }

   ACQUIRE_FN(PerkExpert)
   {
      ++player.perksPerLevel;
   }

   CHECK_FN(PerkMaster)
   {
      auto hasReqs = filterFind(player.acquiredPerks, [](const ActivePerk& perk)
      {
         return perk.type == PerkExpert;
      }) != player.acquiredPerks.size();
      return hasReqs;
   }

   ACQUIRE_FN(MonsterCleaner)
   {
      auto third = player.maxHealth / 3;
      player.maxHealth -= third;
      player.health -= third;
      for( auto& monsta : game.m_monsters )
      {
         monsta->health = 0;
      }
      VKillLocations kills;
      killMonsters(game, kills, false);
      generateSplatter(kills, game);
   }
}
