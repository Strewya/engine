#pragma once
/********************************************
*  contents:   functions for loading, validity check and unloading of game resources
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct AudioSystem;
   struct GameResources;
   struct GraphicsSystem;

   GameResources loadGameResources(AudioSystem& audio, GraphicsSystem& gfx);
   bool checkGameResourcesLoaded(GameResources& assets);
   void unloadGameResources(GameResources& assets, AudioSystem& sounds, GraphicsSystem& gfx);
}
