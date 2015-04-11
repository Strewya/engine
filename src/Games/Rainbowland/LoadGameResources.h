#pragma once
/********************************************
*  contents:   LoadGameResources
*  usage:
********************************************/
/******* C++ headers *******/
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
