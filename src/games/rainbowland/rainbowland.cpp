//headers should be ordered alphabetically!
/******* precompiled header *******/
//#include "stdafx.h"
/******* personal header *******/
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/rainbowland.h"
#include "utility/communication_buffer.h"
#include "utility/memory.h"
/******* end headers *******/

namespace core
{
   core_internal Game* init_game(Allocator& a, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {
      Game* game = a.allocate<Game>();

      return game;
   }

   core_internal bool tickLogic(Game& game, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock& timer)
   {
      return false;
   }

   core_internal void tickRender(Game& game, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock& timer)
   {

   }

   core_internal void shutdown_game(Game& game, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {

   }
}
