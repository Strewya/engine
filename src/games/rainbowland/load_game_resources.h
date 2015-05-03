#pragma once
/********************************************
*  contents:   functions for loading, validity check and unloading of game resources
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class GameResources;
   class GameSystems;

   GameResources loadGameResources(GameSystems&);
   bool checkGameResourcesLoaded(GameResources& assets);
   void unloadGameResources(GameSystems&, GameResources&);
}
