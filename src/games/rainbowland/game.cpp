//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/game.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/camera.h"
#include "graphics/mesh/mesh.h"
#include "graphics/vertex.h"
#include "input/keyboard.h"
#include "utility/color.h"
#include "utility/transform.h"
#include "utility/utility.h"
#include "utility/template/resource_cache_template.h"
#include "utility/time/clock.h"
#include "utility/memory.h"
#include "utility/communication_buffer.h"

#include "games/rainbowland/load_game_resources.h"
#include "games/rainbowland/logic/code.cpp"
/******* end headers *******/


namespace core
{
   core_internal void updateCursorInfo(CommunicationBuffer* toMain, bool show, bool lock, bool relative)
   {
      WinMsg msg;
      msg.type = WinMsgType::ShowCursor;
      msg.showCursor = show;
      toMain->writeEvent(msg);
      msg.type = WinMsgType::LockCursor;
      msg.showCursor = lock;
      toMain->writeEvent(msg);
      msg.type = WinMsgType::RelativeCursor;
      msg.showCursor = relative;
      toMain->writeEvent(msg);
   }

   void Game::onLostFocus()
   {
      isPaused = true;
      updateCursorInfo(toMain, Cursor_Show, Cursor_Unlock, Cursor_Absolute);
   }

   void Game::onGainFocus()
   {
      isPaused = false;
      updateCursorInfo(toMain, game.sharedData.showCursor, game.sharedData.lockCursor, game.sharedData.relativeCursor);
   }

   bool Game::shutdown()
   {
      CORE_SHUTDOWN_START(Rainbowland);

      unloadGameResources(game.assets, audioSystem, graphicsSystem);

      CORE_STATUS_AND(fontSystem.shutdown());
      CORE_STATUS_AND(luaSystem.shutdown());
      CORE_STATUS_AND(inputSystem.shutdown());
      CORE_STATUS_AND(graphicsSystem.shutdown());
      CORE_STATUS_AND(audioSystem.shutdown());

      updateCursorInfo(toMain, Cursor_Show, Cursor_Unlock, Cursor_Absolute);

      CORE_SHUTDOWN_END;
   }

   bool Game::init(CommunicationBuffer* fromMain, CommunicationBuffer* toMain)
   {
      CORE_INIT_START(Rainbowland);

      this->toMain = toMain;
      this->fromMain = fromMain;
      isPaused = false;

      u64 window{};
      WinMsg msg = fromMain->wait();
      CORE_STATUS_AND(msg.type == WinMsgType::WindowHandle);
      window = msg.handle;

      if( CORE_STATUS_OK )
      {
         msg.type = WinMsgType::Size;
         msg.screen.x = msg.screen.y = USE_MONITOR_RESOLUTION;
         toMain->writeEvent(msg);

         msg.type = WinMsgType::FileChange;
         strcpy(msg.fileChange.name, "resources");
         toMain->writeEvent(msg);

         msg.type = WinMsgType::Fullscreen;
         msg.fullscreen = true;
         toMain->writeEvent(msg);

         while( msg.type != WinMsgType::Size )
         {
            msg = fromMain->wait();
            if( msg.type == WinMsgType::LostFocus )
            {
               onLostFocus();
            }
            else if( msg.type == WinMsgType::GainFocus )
            {
               onGainFocus();
            }
         }

         game.constants.windowWidth = (f32)msg.screen.x;
         game.constants.windowHeight = (f32)msg.screen.y;

         FrameAllocator la{};
         //CORE_STATUS_AND(audioSystem.init(la));
         //CORE_STATUS_AND(graphicsSystem.init(window, game.constants.windowWidth, game.constants.windowHeight));
         //CORE_STATUS_AND(inputSystem.init());
         //CORE_STATUS_AND(luaSystem.init());
         //CORE_STATUS_AND(fontSystem.init(graphicsSystem.textures));
      }

      if( CORE_STATUS_OK )
      {
         //CORE_STATUS_AND(init_game(game, audioSystem, graphicsSystem, luaSystem.getStack()));

         //updateCursorStuff(toMain, game.sharedData.showCursor, game.sharedData.lockCursor, game.sharedData.relativeCursor);
      }

      CORE_STATUS_SET(false);
      CORE_INIT_END;
   }

   bool Game::tickLogic(const Clock& logicClock)
   {
      bool running = true;
      std::vector<WinMsg> inputDeviceMsgs;

      WinMsg msg{};
      auto timeLimit = logicClock.getCurrentMicros();
      while( fromMain->peek(msg, timeLimit) )
      {
         switch( msg.type )
         {
            case WinMsgType::KeyboardKey:
            case WinMsgType::KeyboardText:
            case WinMsgType::MouseButton:
            case WinMsgType::MouseMove:
            case WinMsgType::MouseWheel:
            case WinMsgType::GamepadAxis:
            case WinMsgType::GamepadButton:
            case WinMsgType::GamepadConnection:
            {
               inputDeviceMsgs.push_back(msg);
            } break;
            // #refactor this should probably be moved into a system of it's own
            // that handles file change notifications and processes them based on
            // either the extension or the entire file name
            case WinMsgType::FileChange:
            {
               if( msg.fileChange.action == FileChangeType::Modified )
               {
                  //normalize string first, turning \ to /, 
                  char* str = msg.fileChange.name;
                  const char* ext = nullptr;
                  while( *str )
                  {
                     if( *str == '\\' )
                     {
                        *str = '/';
                     }
                     if( *str == '.' )
                     {
                        ext = str + 1;
                     }
                  }

                  if( strcmp(ext, "png") == 0 || strcmp(ext, "bmp") == 0 || strcmp(ext, "tif") == 0 || strcmp(ext, "jpg") == 0 )
                  {
                     //if( graphicsSystem.textures.isLoaded(msg.fileChange.name) )
                     {
                        //graphicsSystem.textures.reloadFromFile(msg.fileChange.name);
                     }
                  }
                  else if( strcmp(ext, "cso") == 0 )
                  {
                     //if( graphicsSystem.pixelShaders.isLoaded(msg.fileChange.name) )
                     {
                        //graphicsSystem.pixelShaders.reloadFromFile(msg.fileChange.name);
                     }
                     //else if( graphicsSystem.vertexShaders.isLoaded(msg.fileChange.name) )
                     {
                        //graphicsSystem.vertexShaders.reloadFromFile(msg.fileChange.name, HealthVertex::getDescription());
                     }
                  }
                  else if( strcmp(ext, "wav") == 0 || strcmp(ext, "mp3") == 0 )
                  {
//                   if( audioSystem.sounds.isLoaded(file.c_str()) )
//                   {
//                      audioSystem.sounds.reloadFromFile(file.c_str());
//                   }
                  }
               }
            } break;
            case WinMsgType::LostFocus:
            {
               onLostFocus();
            } break;
            case WinMsgType::GainFocus:
            {
               onGainFocus();
            } break;
            case WinMsgType::Close:
            {
               return false;
            } break;
         }
      }

      DeltaTime time{};
      time.real.micros = logicClock.getDeltaMicros();
      time.real.seconds = logicClock.getDeltaSeconds();
      time.virt.micros = isPaused ? 0 : time.real.micros;
      time.virt.seconds = isPaused ? 0 : time.real.seconds;

      auto logic_ok = update_game(time, game, inputDeviceMsgs, audioSystem, luaSystem.getStack(), graphicsSystem);

      if( !isPaused )
      {
         updateCursorInfo(toMain, game.sharedData.showCursor, game.sharedData.lockCursor, game.sharedData.relativeCursor);
      }

      luaSystem.collectGarbage();
      audioSystem.update();

      return running && logic_ok;
   }

   void Game::tickRender(const Clock& clock)
   {
      graphicsSystem.begin();

      DeltaTime time{};
      time.real.micros = clock.getDeltaMicros();
      time.real.seconds = clock.getDeltaSeconds();
      time.virt.micros = isPaused ? 0 : time.real.micros;
      time.virt.seconds = isPaused ? 0 : time.real.seconds;

      render_game(time, game, graphicsSystem, fontSystem);

      graphicsSystem.present();
   }
}


