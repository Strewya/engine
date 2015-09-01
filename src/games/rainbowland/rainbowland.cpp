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
   u8* getMemory(MemoryDistributor& distributor, u32 size)
   {
      u8* result = nullptr;
      if( size <= distributor.sizeLeft )
      {
         result = distributor.memory + distributor.offset;
         distributor.offset += size;
         distributor.sizeLeft -= size;
      }
      return result;
   }

   struct InitResult
   {
      GameData* game;
      bool success;
   };
   core_internal InitResult init_game(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {


      return{nullptr, false};
   }

   core_internal bool tickLogic(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock& timer)
   {
      return false;
   }

   core_internal void tickRender(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock& timer)
   {

   }

   core_internal bool shutdown_game(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {
      return true;
   }
}
