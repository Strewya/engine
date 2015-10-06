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

         ExtractNumber(FmodMemoryMegabytes);
         ExtractNumber(FmodMaxChannels);
         ExtractNumber(MaxNumberOfSoundSlots);

         ExtractNumber(MaxNumberOfTextureSlots);
         ExtractNumber(MaxNumberOfShaderSlots);

#undef ExtractNumber

         config.pop();
         memoryConfig->shutdown();
         zeroUsedMemory(mainMemory, mainMemory.remainingBytes - configMemory.remainingBytes);
      }
#endif
      Memory audioMemory = allocateMemoryChunk(mainMemory, MegaBytes(AudioSystemMegabytes), 16);
      Memory graphicsMemory = allocateMemoryChunk(mainMemory, MegaBytes(GraphicsSystemMegabytes), 16);
      Memory scriptMemory = allocateMemoryChunk(mainMemory, MegaBytes(ScriptSystemMegabytes), 16);
      Memory gameMemory = mainMemory;

      CORE_ASSERT_DBGERR(audioMemory != nullptr, "Not enough memory for the audio service.");
      CORE_ASSERT_DBGERR(graphicsMemory != nullptr, "Not enough memory for the graphics service.");
      CORE_ASSERT_DBGERR(scriptMemory != nullptr, "Not enough memory for the script service.");

      AudioSystem* audio = emplace<AudioSystem>(audioMemory);
      audio->init(audioMemory, FmodMemoryMegabytes, FmodMaxChannels, MaxNumberOfSoundSlots);

      GraphicsSystem* graphics = emplace<GraphicsSystem>(graphicsMemory);
      graphics->init(graphicsMemory, MaxNumberOfShaderSlots, MaxNumberOfTextureSlots, windowHandle, windowWidth, windowHeight);

      LuaSystem* script = emplace<LuaSystem>(scriptMemory);
      script->init(scriptMemory);

      Game* game = init_game(gameMemory, fromMain, toMain, audio, graphics, script);

      WinMsg msg{};
      msg.type = WinMsgType::Fullscreen;
      msg.fullscreen = true;
      toMain->writeEvent(msg);
      
      // #todo decide if this should stay enabled in release build
      msg.type = WinMsgType::FileChange;
      strcpy(msg.fileChange.name, "resources");
      toMain->writeEvent(msg);
      //

      auto running = (game != nullptr);
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
            running = tickLogic(fromMain, toMain, &logicTimer, audio, graphics, script, game);
         }
         logicTimer.advanceTimeBy(droppedTime);

         u64 fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
         // we might want to do interpolation ...
         tickRender(fromMain, toMain, &renderTimer, audio, graphics, script, game);
         renderTimer.advanceTimeBy(fullUpdateTime);
      }
      if( game )
      {
         shutdown_game(fromMain, toMain, audio, graphics, script, game);
      }

      script->shutdown();
      graphics->shutdown();
      audio->shutdown();

      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
