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

   void runGame(Allocator& mainMemory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, u64 windowHandle, u32 windowWidth, u32 windowHeight)
   {
      Clock logicTimer{};
      Clock renderTimer{};

#ifdef DEPLOY
      enum : u32
      {
         AudioSystemMemorySize = MegaBytes(40),
         MaxNumberOfSoundSlots = 20,
         ScriptSystemMemorySize = MegaBytes(100),
         GraphicsSystemMemorySize = MegaBytes(50),
         MaxNumberOfTextureSlots = 5,
         //...
      };
#else
      LuaSystem* memoryConfig = mainMemory.allocate<LuaSystem>();
      memoryConfig->init(mainMemory, MegaBytes(1));

      LuaStack config = memoryConfig->getStack();
      bool ok = config.doFile("memory.lua");
      CORE_ASSERT_DBGERR(ok, "Lua configuration file invalid or missing!");
      CORE_ASSERT_DBGERR(config.is<LuaTable>(), "Lua configuration file has invalid structure!");

#define ExtractNumber(name) u32 name = get<u32>(config, #name, 0); CORE_ASSERT_DBGERR(name > 0, "Expected '"#name"' inconfig file, found none or has value 0!")

      ExtractNumber(AudioSystemMegabytes);
      ExtractNumber(GraphicsSystemMegabytes);
      ExtractNumber(ScriptSystemMegabytes);
      ExtractNumber(GameMemoryMegabytes);

      ExtractNumber(FmodMemoryMegabytes);
      ExtractNumber(FmodMaxChannels);
      ExtractNumber(MaxNumberOfSoundSlots);

      ExtractNumber(MaxNumberOfTextureSlots);
      ExtractNumber(MaxNumberOfShaderSlots);

#undef ExtractNumber

      config.pop();
      memoryConfig->shutdown();

      mainMemory.deallocate(memoryConfig);
#endif
      AudioSystem* audio = mainMemory.allocate<AudioSystem>();
      audio->init(mainMemory, MegaBytes(AudioSystemMegabytes), FmodMemoryMegabytes, FmodMaxChannels, MaxNumberOfSoundSlots);

      GraphicsSystem* graphics = mainMemory.allocate<GraphicsSystem>();
      graphics->init(mainMemory, MegaBytes(GraphicsSystemMegabytes), MaxNumberOfShaderSlots, MaxNumberOfTextureSlots, windowHandle, windowWidth, windowHeight);

      LuaSystem* script = mainMemory.allocate<LuaSystem>();
      script->init(mainMemory, MegaBytes(ScriptSystemMegabytes));

      // #todo decide if this should stay enabled in release build
      WinMsg msg{};
      msg.type = WinMsgType::FileChange;
      strcpy(msg.fileChange.name, "resources");
      toMain->writeEvent(msg);
      //

      auto running = true;
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
         CORE_ASSERT_DBGWRN(count == 1, "Doing more than one update per tick, performance warning.");
         while( count-- && running )
         {
            logicTimer.advanceTimeBy(CORE_MICROS_PER_FRAME);
            running = tickLogic(fromMain, toMain, logicTimer);
         }
         logicTimer.advanceTimeBy(droppedTime);

         u64 fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
         // we might want to do interpolation ...
         tickRender(fromMain, toMain, renderTimer);
         renderTimer.advanceTimeBy(fullUpdateTime);
      }
      bool shutdownStatus = shutdown_game(fromMain, toMain);
      if( !shutdownStatus )
      {
         CORE_LOG("Game shutdown failed...");
      }

      //all major systems should have a shutdown method which, in the least, logs the memory usage from the allocators
      audio->shutdown();
      graphics->shutdown();
      script->shutdown();

      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
