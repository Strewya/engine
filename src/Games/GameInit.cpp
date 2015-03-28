//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/GameInit.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Clock.h>
#include <Window/Window.h>

#include <Games/GameList.h>
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


namespace Core
{
   void runGame(Window& window)
   {
#ifndef NO_GAME
      static const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;
      static const uint64_t maxUpdateTime = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL;
      Clock logicTimer;
      Clock renderTimer;
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
            if( !game.tickLogic(microsPerFrame) )
            {
               running = false;
               break;
            }
            logicTimer.updateBy(microsPerFrame);
         }
         logicTimer.updateBy(droppedTime);

         uint64_t fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getLastRealTimeMicros();
         if( count > 0 )
         {
            game.tickRender(fullUpdateTime);
         }
         renderTimer.updateBy(fullUpdateTime);
      }
      game.shutdown();
      window.close();
#endif
   }
}
