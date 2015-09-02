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
      CORE_MICROS_PER_FRAME     = 1000000ULL/CORE_FIXED_STEP_FPS,
      CORE_MAX_MICROS_PER_FRAME = 1000000ULL/CORE_CLAMPED_STEP_MIN_FPS,
      CORE_VARIABLE_STEP = 0,
      CORE_CLAMPED_STEP = 1,
      CORE_FIXED_STEP = 2,
      CORE_STEP = CORE_CLAMPED_STEP,
      CORE_MAX_UPDATE_TIME = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL,
   };

   void runGame(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {
      Clock logicTimer{};
      Clock renderTimer{};
      //initialization phase
      LinearAllocator mainMemory{memory.ptr, memory.size};
      
      AudioSystem* audio = allocate<AudioSystem>(mainMemory);
      audio->init(mainMemory);

      /*
         this function should orchestrate the following sequence of steps:
         1. a filename is provided as to the location of the sound file
         2. the filename is passed to a file loader (instantiated at the spot on the stack)
         3. the file loader simply loads the contents of the file to memory and returns the memory
         4. the memory is passed to a sound loader
         5. the sound loader parses the memory and creates the sound
         6. the sound is returned to the audio system
         7. audio system inserts the sound into cache
         8. cache returns a sound handle
         9. sound handle is mapped with the filename in a name cache
         10. sound handle is returned
      */
      HSound loadedSound = audio->loadFromFile(CORE_RESOURCE("Sounds/default.wav"));

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
            running = tickLogic(memory, fromMain, toMain, logicTimer);
         }
         logicTimer.advanceTimeBy(droppedTime);

         u64 fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
         // we might want to do interpolation ...
         tickRender(memory, fromMain, toMain, renderTimer);
         renderTimer.advanceTimeBy(fullUpdateTime);
      }
      bool shutdownStatus = shutdown_game(memory, fromMain, toMain);
      if( !shutdownStatus )
      {
         CORE_LOG("Game shutdown failed...");
      }

      WinMsg msg{};
      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
