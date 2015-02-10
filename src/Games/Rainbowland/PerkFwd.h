#pragma once
/********************************************
*  contents:   Perk related forward declarations
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/PlayerFwd.h>
/******* end header inclusion *******/

namespace Core
{
   // TEMPORARY 
   class RainbowlandGame;

   enum PerkType : int;

   struct Perk;
   struct ActivePerk;

   typedef std::vector<Perk> VPerks;
   typedef std::vector<ActivePerk> VActivePerks;

#define PERK_CHECK_FN(name) bool name(Player& player)
#define PERK_ACQUIRE_FN(name) void name(Player& player, RainbowlandGame& game)
#define PERK_UPDATE_FN(name) void name(ActivePerk& aperk, Player& player, RainbowlandGame& game)

   typedef PERK_CHECK_FN(CheckPerk);
   typedef PERK_ACQUIRE_FN(AcquirePerk);
   typedef PERK_UPDATE_FN(UpdatePerk);
}
