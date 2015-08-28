//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/game_entry_point.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/time/clock.h"
#include "utility/communication_buffer.h"
#include "utility/memory.h"
#include "utility/utility.h"
#include "window/window_message.h"

#include "games/game_list.h"
#include CORE_HEADER
/******* end headers *******/

enum
{
   CORE_FIXED_STEP_FPS = 60ULL,
   CORE_CLAMPED_STEP_MIN_FPS = 15ULL,
   CORE_MICROS_PER_FRAME     = 1000000ULL/CORE_FIXED_STEP_FPS,
   CORE_MAX_MICROS_PER_FRAME = 1000000ULL/CORE_CLAMPED_STEP_MIN_FPS,
   CORE_VARIABLE_STEP = 0,
   CORE_CLAMPED_STEP = 1,
   CORE_FIXED_STEP = 2,
   CORE_STEP = CORE_CLAMPED_STEP,
};

namespace core
{
   void runGame(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {
#ifndef NO_GAME
      enum
      {
         microsPerFrame = CORE_MICROS_PER_FRAME,
         maxUpdateTime = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL,
      };
      Clock logicTimer{};
      Clock renderTimer{};
      CORE_GAME game;

      bool running = game.init(fromMain, toMain);
      while( running )
      {
         f32 fraction = 0;
         u64 unusedMicros = 0;
         u64 droppedTime = 0;

         const u32 maxUpdateCount = static_cast<u32>(maxUpdateTime / microsPerFrame);
         const u32 updateCount = logicTimer.getFixedStepUpdateCount(microsPerFrame, fraction, unusedMicros);
         droppedTime = updateCount > maxUpdateCount ? updateCount - maxUpdateCount : 0;
         droppedTime *= microsPerFrame;

         u32 count, l;
         count = l = (updateCount <= maxUpdateCount ? updateCount : maxUpdateCount);
         while( l-- )
         {
            logicTimer.advanceTimeBy(microsPerFrame);
            if( !game.tickLogic(logicTimer) )
            {
               running = false;
               break;
            }
         }
         logicTimer.advanceTimeBy(droppedTime);

         u64 fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
         // we might want to do interpolation ...
         game.tickRender(renderTimer);
         renderTimer.advanceTimeBy(fullUpdateTime);
      }
      bool shutdownStatus = game.shutdown();
      if( !shutdownStatus )
      {
         CORE_LOG("Game shutdown failed...");
      }
#else
      static_assert(false, "No game defined");
#endif
      WinMsg msg{};
      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
