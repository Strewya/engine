#pragma once
/********************************************
*  contents:   Weapon related structures
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
#include <Games/Rainbowland/WeaponFwd.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   enum WeaponType : int
   {
      Pistol,
      Shotgun,
      Uzi,
      Sniper,
      RPG,
      WeaponTypeCount
   };

   struct Weapon
   {
      WeaponType type;
      std::string name;
      uint32_t fireDelay;
      uint32_t reloadDelay;
      uint32_t damage;
      int32_t ammo;
      uint32_t maxAmmo;
      uint32_t bulletsPerShot;
      float spread;
      bool bulletPierce;
   };
}
