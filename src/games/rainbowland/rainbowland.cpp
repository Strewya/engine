//headers should be ordered alphabetically!
/******* precompiled header *******/
//#include "stdafx.h"
/******* personal header *******/
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/types.h"
#include "utility/communication_buffer.h"
#include "utility/memory.h"
/******* end headers *******/

namespace core
{
   void* allocate(BlockAllocator& allocator, u32 size, u32 align)
   {
   }

   core_internal bool init_game(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {
      return false;
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
