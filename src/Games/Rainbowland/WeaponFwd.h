#pragma once
/********************************************
*  contents:   Weapon related forward declarations
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   enum WeaponType : int;

   struct Weapon;

   typedef std::vector<Weapon> VWeapons;
}
