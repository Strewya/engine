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

   void runGame(LinearAllocator& mainMemory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, u64 windowHandle)
   {
      Clock logicTimer{};
      Clock renderTimer{};
      //initialization phase



      //temporary until i get the lua state up, then for development use the config file, and for release use hardcoded values based on profiling memory usage
#ifdef DEPLOY
      enum : u32
      {
         AudioSystemMemorySize = Megabytes(40),
         MaxNumberOfSoundSlots = 20,
         LuaSystemMemorySize = Megabytes(100),
         GraphicsSystemMemorySize = Megabytes(50),
         MaxNumberOfTextureSlots = 5,
         //...
      };
#else
      //allocate a temporary lua state that reads a config file containing various sizes, such as:
      LinearAllocator luaTemporaryAllocator = mainMemory;
      LuaSystem* luaConfigReader = allocate<LuaSystem>(luaTemporaryAllocator);
      luaConfigReader->init(luaTemporaryAllocator, Megabytes(10));

      LuaStack config = luaConfigReader->getStack();
      bool ok = config.doFile("config.lua");
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, ok, "Lua configuration file invalid or missing!");
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, config.is<LuaTable>());

      u32 AudioSystemMegabytes = get<u32>(config, "AudioSystemMegabytes", 0);
      u32 MaxNumberOfSoundSlots = get<u32>(config, "MaxNumberOfSoundSlots", 0);
      u32 LuaSystemMegabytes = get<u32>(config, "LuaSystemMegabytes", 0);
      u32 GraphicsSystemMegabytes = get<u32>(config, "GraphicsSystemMegabytes", 0);
      u32 MaxNumberOfTextureSlots = get<u32>(config, "MaxNumberOfTextureSlots", 0);

      config.pop();
      luaConfigReader->shutdown();
      memset(mainMemory.memory + mainMemory.allocated, 0, luaTemporaryAllocator.allocated - mainMemory.allocated);
#endif

      CORE_ASSERT_DEBUG(AssertLevel::Fatal, AudioSystemMegabytes > 0);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, MaxNumberOfSoundSlots > 0);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, LuaSystemMegabytes > 0);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, GraphicsSystemMegabytes > 0);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, MaxNumberOfTextureSlots > 0);

      AudioSystem* audio = allocate<AudioSystem>(mainMemory);
      audio->init(mainMemory, Megabytes(AudioSystemMegabytes), MaxNumberOfSoundSlots);

      LuaSystem* lua = allocate<LuaSystem>(mainMemory);
      lua->init(mainMemory, Megabytes(LuaSystemMegabytes));
      
      /*
      GraphicsSystem* graphics = allocate<GraphicsSystem>(graphicsMemory);
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

      audio->shutdown();
      lua->shutdown();

      WinMsg msg{};
      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
