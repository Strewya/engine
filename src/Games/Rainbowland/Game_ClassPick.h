#pragma once
/********************************************
*  contents:   Gameplay - class pick
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
   class Gameplay_ClassPick
   {
   public:
      std::vector<HPlayer> playerHandles;

   };

   class GameState;

   void updateClassPicking(GameState& state);
}