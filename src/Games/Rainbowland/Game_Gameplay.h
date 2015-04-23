#pragma once
/********************************************
*  contents:   Gameplay
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Game_ClassPick.h>
#include <Games/Rainbowland/Game_ColorPick.h>
#include <Games/Rainbowland/Gameplay_Session.h>
/******* end header inclusion *******/

namespace Core
{
   class Gameplay
   {
   public:
      Gameplay_ColorPick colorPick;
      Gameplay_ClassPick classPick;
      Gameplay_Session session;

   };
}