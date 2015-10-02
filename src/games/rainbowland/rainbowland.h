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

   struct Game
   {
      Memory gameMemory;
      World world;
      //GameAssets assets;
      //...
   };
}
