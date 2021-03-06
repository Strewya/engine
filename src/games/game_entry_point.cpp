//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/game_entry_point.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/audio_system.h"
#include "graphics/graphics_system.h"
#include "input/input_system.h"
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
      u32 InputSystemMegabytes;
      u32 ScriptSystemMegabytes;

      u32 FmodMemoryMegabytes;
      u32 FmodMaxChannels;
      u32 MaxNumberOfSoundSlots;

      u32 MaxNumberOfTextureSlots;
      u32 MaxNumberOfShaderSlots;

      {
         Memory configMemory = mainMemory;
         LuaSystem* L = script::init(configMemory);

         auto luaFile = script::openConfigFile(L, "memory.lua");
         CORE_ASSERT_DBGERR(luaFile.index != 0, "Lua configuration file invalid or missing!");
         
#define ExtractNumber(name) name = script::readNamedValue(L, luaFile, #name, (u32)0); CORE_ASSERT_DBGERR(name > 0, "Expected '"#name"' in config file, found none or has value 0!")

         ExtractNumber(AudioSystemMegabytes);
         ExtractNumber(GraphicsSystemMegabytes);
         ExtractNumber(InputSystemMegabytes);
         ExtractNumber(ScriptSystemMegabytes);

         ExtractNumber(FmodMemoryMegabytes);
         ExtractNumber(FmodMaxChannels);
         ExtractNumber(MaxNumberOfSoundSlots);

         ExtractNumber(MaxNumberOfTextureSlots);
         ExtractNumber(MaxNumberOfShaderSlots);

#undef ExtractNumber

         script::closeConfigFile(L, luaFile);

         script::shutdown(L);
         zeroUsedMemory(mainMemory, mainMemory.remainingBytes - configMemory.remainingBytes);
      }
#endif
      Memory audioMemory = allocateMemoryChunk(mainMemory, MegaBytes(AudioSystemMegabytes), 16);
      Memory graphicsMemory = allocateMemoryChunk(mainMemory, MegaBytes(GraphicsSystemMegabytes), 16);
      Memory inputMemory = allocateMemoryChunk(mainMemory, MegaBytes(InputSystemMegabytes), 16);
      Memory scriptMemory = allocateMemoryChunk(mainMemory, MegaBytes(ScriptSystemMegabytes), 16);
      Memory gameMemory = mainMemory;

      CORE_ASSERT_DBGERR(audioMemory != nullptr, "Not enough memory for the audio service.");
      CORE_ASSERT_DBGERR(graphicsMemory != nullptr, "Not enough memory for the graphics service.");
      CORE_ASSERT_DBGERR(inputMemory != nullptr, "Not enough memory for the input service.");
      CORE_ASSERT_DBGERR(scriptMemory != nullptr, "Not enough memory for the script service.");

      AudioSystem* sfx = audio::init(audioMemory, FmodMemoryMegabytes, FmodMaxChannels, MaxNumberOfSoundSlots);
      GraphicsSystem* gfx = graphics::init(graphicsMemory, MaxNumberOfShaderSlots, MaxNumberOfTextureSlots, windowHandle, windowWidth, windowHeight);
      InputSystem* input = input::init(inputMemory);
      LuaSystem* lua = script::init(scriptMemory);
      Game* game = initGame(gameMemory, fromMain, toMain, sfx, gfx, input, lua);

      CORE_ASSERT_DBGERR(sfx != nullptr, "Failed to initialize audio subsystem");
      CORE_ASSERT_DBGERR(gfx != nullptr, "Failed to initialize graphics subsystem");
      CORE_ASSERT_DBGERR(input != nullptr, "Failed to initialize input subsystem");
      CORE_ASSERT_DBGERR(lua != nullptr, "Failed to initialize script subsystem");
      CORE_ASSERT_DBGERR(game != nullptr, "Failed to initialize game");

      WinMsg msg{};
      msg.type = WinMsgType::Fullscreen;
      msg.fullscreen = true;
      toMain->writeEvent(msg);

      // #todo decide if this should stay enabled in release build
      msg.type = WinMsgType::FileChange;
      strcpy(msg.fileChange.name, "resources");
      toMain->writeEvent(msg);
      //

      Clock logicTimer{};
      Clock renderTimer{};
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
         CORE_ASSERT_DBGWRN(count == 1, "Doing ", count, " updates per tick instead of one, performance warning.");
         while( count-- && running )
         {
            logicTimer.advanceTimeBy(CORE_MICROS_PER_FRAME);
            running = tickLogic(fromMain, toMain, &logicTimer, sfx, gfx, input, lua, game);
         }
         logicTimer.advanceTimeBy(droppedTime);
         
         if( running )
         {
            u64 fullUpdateTime = logicTimer.getLastRealTimeMicros() + unusedMicros - renderTimer.getCurrentMicros();
            // we might want to do interpolation ...
            tickRender(fromMain, toMain, &renderTimer, sfx, gfx, lua, game);
            renderTimer.advanceTimeBy(fullUpdateTime);
         }
      }

      shutdownGame(fromMain, toMain, sfx, gfx, input, lua, game);
      script::shutdown(lua);
      graphics::shutdown(gfx);
      audio::shutdown(sfx);

      msg.type = WinMsgType::Close;
      toMain->writeEvent(msg);
   }
}
