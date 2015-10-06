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
   struct GameAssets
   {
      HTexture atlas;
      HTexture background;
      HTexture font;
      HVertexShader mainVS;
      HPixelShader mainPS;
      HPixelShader healthPS;
      HSound reload;
      HSound pistol;
      HSound uzi;
      HSound rpg;
      HSound sniper;
      HSound shotgun;
   };

   struct World
   {

   };

   struct Game
   {
      Memory gameMemory;
      GameAssets assets;
      World world;
      //...
   };
}
