//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/WeaponDatabase.h>
/******* C++ headers *******/
#include <algorithm>
#include <cassert>
/******* extra headers *******/
#include <Games/Rainbowland/Weapon.h>
#include <Util/Time.h>
/******* end headers *******/

namespace Core
{
   void initWeaponDatabase(VWeapons& weapons)
   {
      weapons =
      {
         {Pistol},
         {Shotgun},
         {Uzi},
         {Sniper},
         {RPG},
      };

      assert(weapons.size() == (uint32_t)WeaponTypeCount);

      std::sort(weapons.begin(), weapons.end(),
                [](const Weapon& l, const Weapon& r)
      {
         return l.type < r.type;
      });
      
      WeaponType 
      type = Pistol;
      weapons[type].name            = "Pew-Pew";
      weapons[type].fireDelay       = secondsToMicros(0.5f);
      weapons[type].reloadDelay     = secondsToMicros(0.7f);
      weapons[type].damage          = 40;
      weapons[type].maxAmmo         = 20;
      weapons[type].bulletsPerShot  = 1;
      weapons[type].spread          = 0;
      weapons[type].bulletPierce    = false;
      
      type = Shotgun;
      weapons[type].name            = "Spreader";
      weapons[type].fireDelay       = secondsToMicros(0.5f);
      weapons[type].reloadDelay     = secondsToMicros(1.2f);
      weapons[type].damage          = 80;
      weapons[type].maxAmmo         = 12;
      weapons[type].bulletsPerShot  = 6;
      weapons[type].spread          = 2;
      weapons[type].bulletPierce    = false;

      type = Uzi;
      weapons[type].name            = "Urban cleaner";
      weapons[type].fireDelay       = secondsToMicros(0.1f);
      weapons[type].reloadDelay     = secondsToMicros(1.3f);
      weapons[type].damage          = 70;
      weapons[type].maxAmmo         = 32;
      weapons[type].bulletsPerShot  = 1;
      weapons[type].spread          = 0.5f;
      weapons[type].bulletPierce    = false;

      type = Sniper;
      weapons[type].name            = "Piercer";
      weapons[type].fireDelay       = secondsToMicros(0.5f);
      weapons[type].reloadDelay     = secondsToMicros(1.0f);
      weapons[type].damage          = 100;
      weapons[type].maxAmmo         = 8;
      weapons[type].bulletsPerShot  = 1;
      weapons[type].spread          = 0;
      weapons[type].bulletPierce    = true;

      type = RPG;
      weapons[type].name            = "Bazoooooka";
      weapons[type].fireDelay       = secondsToMicros(0.3f);
      weapons[type].reloadDelay     = secondsToMicros(1.5f);
      weapons[type].damage          = 70;
      weapons[type].maxAmmo         = 10;
      weapons[type].bulletsPerShot  = 1;
      weapons[type].spread          = 0;
      weapons[type].bulletPierce    = false;
   }
}
