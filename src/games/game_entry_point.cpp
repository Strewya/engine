//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/game_entry_point.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "util/time/clock.h"
#include "util/utility.h"
#include "window/window_proxy.h"

#include "games/game_list.h"
#include CORE_HEADER
/******* end headers *******/

#define CORE_FIXED_STEP_FPS 60ULL
#define CORE_CLAMPED_STEP_MIN_FPS 15ULL

#define CORE_MICROS_PER_FRAME     1000000ULL/CORE_FIXED_STEP_FPS
#define CORE_MAX_MICROS_PER_FRAME 1000000ULL/CORE_CLAMPED_STEP_MIN_FPS

#define CORE_VARIABLE_STEP 0
#define CORE_CLAMPED_STEP 1
#define CORE_FIXED_STEP 2
#define CORE_STEP CORE_CLAMPED_STEP


namespace core
{
   void runGame(WindowProxy window)
   {
#ifndef NO_GAME
#ifndef DEPLOY
      window.openConsole(10, 10);
#endif
      static const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;
      static const uint64_t maxUpdateTime = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL;
      Clock logicTimer{};
      Clock renderTimer{};
      CORE_GAME game;

      bool running = game.init(window);
      while( running && window.isRunning() )
      {
         float fraction = 0;
         uint64_t unusedMicros = 0;
         uint64_t droppedTime = 0;
         
         const uint32_t maxUpdateCount = static_cast<uint32_t>(maxUpdateTime / microsPerFrame);
         const uint32_t updateCount = logicTimer.getFixedStepUpdateCount(microsPerFrame, fraction, unusedMicros);
         droppedTime = updateCount > maxUpdateCount ? updateCount - maxUpdateCount : 0;
         droppedTime *= microsPerFrame;
         
         uint32_t count, l;
         count = l = (updateCount <= maxUpdateCount ? updateCount : maxUpdateCount);
         for( l; l--; )
         {
            logicTimer.advanceTimeBy(microsPerFrame);
            if( !game.tickLogic(logicTimer) )
            {
               running = false;
               break;
            }
         }
         logicTimer.advanceTimeBy(droppedTime);

         uint64_t fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
         // we might want to do interpolation ...
         game.tickRender(renderTimer);
         renderTimer.advanceTimeBy(fullUpdateTime);
      }
      bool shutdownStatus = game.shutdown();
      if( !shutdownStatus )
      {
         CORE_INFO("GAME SHUTDOWN HAS FAILED!!!");
         window.showMessagebox("SRS ERRORR", "Game shutdown has failed, please review the log for errors immediately");
      }
      window.close();
#endif
   }
}
