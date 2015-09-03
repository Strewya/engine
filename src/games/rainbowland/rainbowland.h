#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/graphics_system.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct World
   {

   };

   struct GameState
   {
      World world;
      //GameAssets assets;
      //...
   };

   struct GameData
   {
      GameState game;
      GraphicsSystem gfx;
      //AudioSystem* audio;
      //...
   };
}
