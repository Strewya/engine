#pragma once
/********************************************
*  contents:   Perk related structure declarations
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
#include <Games/Rainbowland/PerkFwd.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   enum PerkType : int
   {
      Regeneration,
      GreaterRegeneration,
      PoisonBullets,
      Radioactive,
      AmmoManiac,
      InstantWinner,
      Fastloader,
      ReflexBoosted,
      Dodger,
      Ninja,
      Assassin,
      HealthBoost,
      EquipedWeaponDamageBoost,
      EquipedWeaponAmmoBoost,
      FasterCooldown,
      HotTempered,
      AngryReloader,
      StationaryReloader,
      FastShot,
      ExtraBullet,
      PerkExpert,
      PerkMaster,
      MonsterCleaner,
      PerkTypeCount
   };

   struct Perk
   {
      PerkType type;
      std::string name;
      std::string description;
      CheckPerk* dependencyCheck;
      AcquirePerk* acquireLogic;
      UpdatePerk* updateLogic;
      bool repeatable;
   };

   struct ActivePerk
   {
      PeriodicTimer timer;
      PerkType type;
   };
}
