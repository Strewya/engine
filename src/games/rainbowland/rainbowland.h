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
   struct MemoryDistributor
   {
      MemoryDistributor(Memory m) : memory{(u8*)m.ptr}, offset{0}, sizeLeft{m.size} {}

      u8* memory;
      u32 offset;
      u32 sizeLeft;
   };



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
