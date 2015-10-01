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



   void runGame(Memory mainMemory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, u64 windowHandle, u32 windowWidth, u32 windowHeight)
   {
      Clock logicTimer{};
      Clock renderTimer{};

#ifdef DEPLOY
      enum : u32
      {
         AudioSystemMegabytes = 40,
         GraphicsSystemMegabytes = 40,
         ScriptSystemMegabytes = 40,
         GameMemoryMegabytes = 500,

         FmodMemoryMegabytes = 30,
         FmodMaxChannels = 32,
         MaxNumberOfSoundSlots = 10,

         MaxNumberOfTextureSlots = 5,
         MaxNumberOfShaderSlots = 5,
      };
#else
      u32 AudioSystemMegabytes;
      u32 GraphicsSystemMegabytes;
      u32 ScriptSystemMegabytes;
      u32 GameMemoryMegabytes;

      u32 FmodMemoryMegabytes;
      u32 FmodMaxChannels;
      u32 MaxNumberOfSoundSlots;

      u32 MaxNumberOfTextureSlots;
      u32 MaxNumberOfShaderSlots;

      {
         Memory configMemory = mainMemory;
         LuaSystem* memoryConfig = emplace<LuaSystem>(configMemory);
         memoryConfig->init(configMemory);
         
         LuaStack config = memoryConfig->getStack();
         bool ok = config.doFile("memory.lua");
         CORE_ASSERT_DBGERR(ok, "Lua configuration file invalid or missing!");
         CORE_ASSERT_DBGERR(config.is<LuaTable>(), "Lua configuration file has invalid structure!");

#define ExtractNumber(name) name = get<u32>(config, #name, 0); CORE_ASSERT_DBGERR(name > 0, "Expected '"#name"' in config file, found none or has value 0!")

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
      }
#endif

      auto before = __rdtsc();

      Memory audioMemory = alignMemory(mainMemory, 16);
      Memory graphicsMemory;
      Memory scriptMemory;
      Memory gameMemory;


      auto fragmentationLostBytes = mainMemory.remainingBytes - audioMemory.remainingBytes;
      CORE_ASSERT_DBGWRN(fragmentationLostBytes == 0, "Losing ", fragmentationLostBytes, " bytes due to alignment");
      mainMemory = advanceMemory(audioMemory, MegaBytes(AudioSystemMegabytes));
      
      AudioSystem* audio = emplace<AudioSystem>(audioMemory);
      audio->init(audioMemory, FmodMemoryMegabytes, FmodMaxChannels, MaxNumberOfSoundSlots);

      FrameAllocator* graphicsMemory = mainMemory.allocate<FrameAllocator>();
      graphicsMemory->acquireMemory(mainMemory, MegaBytes(GraphicsSystemMegabytes));
      GraphicsSystem* graphics = graphicsMemory->allocate<GraphicsSystem>();
      graphics->init(*graphicsMemory, MaxNumberOfShaderSlots, MaxNumberOfTextureSlots, windowHandle, windowWidth, windowHeight);

      HeapAllocator* scriptMemory = mainMemory.allocate<HeapAllocator>();
      scriptMemory->acquireMemory(mainMemory, MegaBytes(ScriptSystemMegabytes));
      LuaSystem* script = scriptMemory->allocate<LuaSystem>();
      script->init(*scriptMemory);
      
      FrameAllocator* gameMemory = mainMemory.allocate<FrameAllocator>();
      gameMemory->acquireMemory(mainMemory, MegaBytes(GameMemoryMegabytes));
      Game* game = init_game(*gameMemory, fromMain, toMain);

      // #todo decide if this should stay enabled in release build
      WinMsg msg{};
      msg.type = WinMsgType::FileChange;
      strcpy(msg.fileChange.name, "resources");
      toMain->writeEvent(msg);
      //

      auto running = (game == nullptr);
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
            running = tickLogic(*game, fromMain, toMain, logicTimer);
         }
         logicTimer.advanceTimeBy(droppedTime);

         u64 fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
         // we might want to do interpolation ...
         tickRender(*game, fromMain, toMain, renderTimer);
         renderTimer.advanceTimeBy(fullUpdateTime);
      }
      if( game )
      {
         shutdown_game(*game, fromMain, toMain);
         gameMemory->deallocate(game);
         gameMemory->returnMemory(mainMemory);
         mainMemory.deallocate(gameMemory);
      }

      script->shutdown();
      scriptMemory->deallocate(script);
      scriptMemory->returnMemory(mainMemory);
      mainMemory.deallocate(scriptMemory);

      graphics->shutdown();
      graphicsMemory->deallocate(graphics);
      graphicsMemory->returnMemory(mainMemory);
      mainMemory.deallocate(graphicsMemory);

      audio->shutdown();
      audioMemory->deallocate(audio);
      audioMemory->returnMemory(mainMemory);
      mainMemory.deallocate(audioMemory);

      auto after = __rdtsc();
      CORE_LOG("All systems init and shutdown took ", (after - before), " cycles");


      before = __rdtsc();
      struct Bleb
      {
         f64 d;
         f32 f;
         u32 i;
      };
      HeapAllocator* testHeap = mainMemory.allocate<HeapAllocator>();
      testHeap->acquireMemory(mainMemory, KiloBytes(1));
      for( u32 i = 1; i != 0; ++i )
      {
         auto* t = testHeap->allocate<Bleb>();
         testHeap->deallocate(t);
      }
      after = __rdtsc();
      CORE_LOG("Stupid took ", (after - before), " cycles");

      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
