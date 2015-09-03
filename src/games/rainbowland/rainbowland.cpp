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
   struct InitResult
   {
      GameData* game;
      bool success;
   };

   core_internal InitResult init_game(LinearAllocator memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {


      return{nullptr, false};
   }

   core_internal bool tickLogic(LinearAllocator memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock& timer)
   {
      return false;
   }

   core_internal void tickRender(LinearAllocator memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock& timer)
   {

   }

   core_internal bool shutdown_game(LinearAllocator memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {
      return true;
   }
}
