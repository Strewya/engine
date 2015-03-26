#pragma once
/********************************************
*  contents:   Movement code
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/MovementSystem.h>
#include <Games/Rainbowland/TimingSystem.h>
/******* end header inclusion *******/

namespace Core
{
   struct GameplaySystems
   {
      MovementSystem movement;
      TimingSystem timing;
   };
}
