#pragma once
/********************************************
*  contents:   Abilities
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Weapon.h>
#include <Util/Circle.h>
#include <Util/Time.h>
/******* end header inclusion *******/

namespace Core
{
   enum AbilityType
   {
      AT_DefenseMatrix,
      AT_TimeCapsule,
      AT_HealingCloud,
      AT_Turret
   };

   struct DefenseMatrix
   {
      CooldownTimer timer;
      float cooldownSeconds;
      float durationSeconds;
      Circle area;
      bool active;
   };

   struct TimeCapsule
   {
      CooldownTimer timer;
      float cooldownSeconds;
      float durationSeconds;
      Circle area;
      bool active;
   };

   struct HealingCloud
   {
      CooldownTimer timer;
      float cooldownSeconds;
      float durationSeconds;
      PeriodicTimer healTimer;
      uint32_t healStrength;
      Circle area;
      bool active;
   };

   struct Turret
   {
      CooldownTimer timer;
      float cooldownSeconds;
      float durationSeconds;
      Circle area;
      Weapon weapon;
      PeriodicTimer weaponTimer;
      Vec2f aim;
      uint32_t killCount;
      bool active;
   };
}
