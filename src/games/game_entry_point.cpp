//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/game_entry_point.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/audio_system.h"
#include "graphics/graphics_system.h"
#include "lua/lua_system.h"
#include "utility/time/clock.h"
#include "utility/communication_buffer.h"
#include "utility/memory.h"
#include "utility/utility.h"
#include "window/window_message.h"


#include "games/rainbowland/rainbowland.cpp"
/******* end headers *******/

namespace core
{
   enum
   {
      CORE_FIXED_STEP_FPS = 60ULL,
      CORE_CLAMPED_STEP_MIN_FPS = 15ULL,
      CORE_MICROS_PER_FRAME = 1000000ULL / CORE_FIXED_STEP_FPS,
      CORE_MAX_MICROS_PER_FRAME = 1000000ULL / CORE_CLAMPED_STEP_MIN_FPS,
      CORE_VARIABLE_STEP = 0,
      CORE_CLAMPED_STEP = 1,
      CORE_FIXED_STEP = 2,
      CORE_STEP = CORE_CLAMPED_STEP,
      CORE_MAX_UPDATE_TIME = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL,
   };

   void runGame(LinearAllocator mainMemory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, u64 windowHandle)
   {
      Clock logicTimer{};
      Clock renderTimer{};
      //initialization phase

      //temporary until i get the lua state up, then for development use the config file, and for release use hardcoded values based on profiling memory usage
#ifndef DEPLOY
      enum
      {
         AudioMemorySize = Megabytes(40),
         MaxNumberOfSoundSlots = 20,
         GraphicsMemorySize = Megabytes(50),
         MaxNumberOfTextureSlots = 5,
         //...
      };
#else
      //allocate a temporary lua state that reads a config file containing various sizes, such as:
      //1. audio memory count
      //2. number of sound slots available
      //3. graphics memory
      //4. texture slots
      //5. shader slots
      //...
#endif

      AudioSystem* audio = allocate<AudioSystem>(mainMemory);
      audio->init(mainMemory, AudioMemorySize, MaxNumberOfSoundSlots);

      HSound loadedSound = audio->loadFromFile(CORE_RESOURCE("Sounds/default.wav"));

      audio->shutdown();

      /*

            GraphicsSystem* graphics = allocate<GraphicsSystem>(graphicsMemory);
            LuaSystem* lua = allocate<LuaSystem>(luaMemory);
            GameState* game = allocate<GameState>(gameStateMemory);
            */

      auto running = false;
      while( running )
      {
         f32 fraction = 0;
         u64 unusedMicros = 0;
         u64 droppedTime = 0;

         const u32 maxUpdateCount = static_cast<u32>(CORE_MAX_UPDATE_TIME / CORE_MICROS_PER_FRAME);
         const u32 updateCount = logicTimer.getFixedStepUpdateCount(CORE_MICROS_PER_FRAME, fraction, unusedMicros);
         droppedTime = updateCount > maxUpdateCount ? updateCount - maxUpdateCount : 0;
         droppedTime *= CORE_MICROS_PER_FRAME;

         u32 count;
         count = (updateCount <= maxUpdateCount ? updateCount : maxUpdateCount);
         CORE_ASSERT_DEBUG(AssertLevel::Notification, count == 1, "Lag spike...");
         while( count-- && running )
         {
            logicTimer.advanceTimeBy(CORE_MICROS_PER_FRAME);
            running = tickLogic(mainMemory, fromMain, toMain, logicTimer);
         }
         logicTimer.advanceTimeBy(droppedTime);

         u64 fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
         // we might want to do interpolation ...
         tickRender(mainMemory, fromMain, toMain, renderTimer);
         renderTimer.advanceTimeBy(fullUpdateTime);
      }
      bool shutdownStatus = shutdown_game(mainMemory, fromMain, toMain);
      if( !shutdownStatus )
      {
         CORE_LOG("Game shutdown failed...");
      }

      WinMsg msg{};
      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
