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
#include "util/color.h"
#include "util/transform.h"
#include "util/utility.h"
#include "util/template/resource_cache_template.h"
#include "util/time/clock.h"

#include "games/rainbowland/load_game_resources.h"
#include "games/rainbowland/logic/code.cpp"
/******* end headers *******/


namespace core
{
   static const bool Lock = true;
   static const bool Unlock = false;
   static const bool Hide = false;
   static const bool Show = true;
   static const bool Absolute = false;
   static const bool Relative = true;

   static void updateCursorStuff(WindowProxy window, bool show, bool lock, bool relative)
   {
      window.makeMouseRelative(relative);
      window.showCursor(show);
      window.lockCursor(lock);
   }

   bool Game::shutdown()
   {
      CORE_SHUTDOWN_START(Rainbowland);

      unloadGameResources(game.assets, audioSystem.sounds, graphicsSystem.pixelShaders, graphicsSystem.vertexShaders, graphicsSystem.textures);

      CORE_STATUS_AND(fontSystem.shutdown());
      CORE_STATUS_AND(luaSystem.shutdown());
      CORE_STATUS_AND(inputSystem.shutdown());
      CORE_STATUS_AND(graphicsSystem.shutdown());
      CORE_STATUS_AND(audioSystem.shutdown());

      updateCursorStuff(window, Show, Unlock, Absolute);

      CORE_SHUTDOWN_END(Rainbowland);
   }

   bool Game::init(WindowProxy windowProxy)
   {
      CORE_INIT_START(Rainbowland);

      this->window = windowProxy;
      isPaused = false;

      CORE_STATUS_AND(window.isValid());
      if( CORE_STATUS_OK )
      {
         window.resize(USE_MONITOR_RESOLUTION, USE_MONITOR_RESOLUTION);
         //#ifdef MURRAY
         //window.move(window.getSizeX(), 0);
         //#endif
         window.monitorDirectoryForChanges("resources");

         window.setFullscreen(true);

         CORE_STATUS_AND(audioSystem.init());
         CORE_STATUS_AND(graphicsSystem.init(window));
         CORE_STATUS_AND(inputSystem.init(window));
         CORE_STATUS_AND(luaSystem.init());
         CORE_STATUS_AND(fontSystem.init(graphicsSystem.textures));
      }

      if( CORE_STATUS_OK )
      {
         game.constants.windowWidth = (float)window.getSizeX();
         game.constants.windowHeight = (float)window.getSizeY();

         CORE_STATUS_AND(init_game(game, audioSystem, graphicsSystem, luaSystem.getStack()));

         updateCursorStuff(window, game.sharedData.showCursor, game.sharedData.lockCursor, game.sharedData.relativeCursor);
      }

      CORE_INIT_END(Rainbowland);
   }

   bool Game::tickLogic(const Clock& logicClock)
   {
      bool running = true;

      //call service updates that the game shouldn't know anything about
      inputSystem.gatherInputForCurrentFrame(logicClock.getCurrentMicros());

      // #todo think about where this part should be, out here or in the game
      const auto& frameEvents = inputSystem.getEvents();
      for( auto& e : frameEvents )
      {
         switch( e.type )
         {
            // #refactor this should probably be moved into a system of it's own
            // that handles file change notifications and processes them based on
            // either the extension or the entire file name
            case WE_FILECHANGE:
            {
               if( e.fileChange.action == FILE_MODIFIED )
               {
                  auto getExtension = [](const std::string& str) -> std::string
                  {
                     auto lastDot = str.find_last_of('.');
                     return str.substr(lastDot + 1);
                  };
                  std::string file = replaceAll(e.fileChange.filename, "\\", "/");
                  file = "../resources/" + file;
                  auto ext = getExtension(file);
                  if( ext == "png" || ext == "bmp" || ext == "tif" || ext == "jpg" )
                  {
                     if( graphicsSystem.textures.isLoaded(file.c_str()) )
                     {
                        graphicsSystem.textures.reloadFromFile(file.c_str());
                     }
                  }
                  else if( ext == "cso" )
                  {
                     if( graphicsSystem.pixelShaders.isLoaded(file.c_str()) )
                     {
                        graphicsSystem.pixelShaders.reloadFromFile(file.c_str());
                     }
                     else if( graphicsSystem.vertexShaders.isLoaded(file.c_str()) )
                     {
                        graphicsSystem.vertexShaders.reloadFromFile(file.c_str(), HealthVertex::getDescription());
                     }
                  }
                  else if( ext == "wav" || ext == "mp3" )
                  {
                     if( audioSystem.sounds.isLoaded(file.c_str()) )
                     {
                        audioSystem.sounds.reloadFromFile(file.c_str());
                     }
                  }
               }
            } break;

            // #todo think about if this should stay
            case WE_LOSTFOCUS:
            {
               isPaused = true;
               updateCursorStuff(window, Show, Unlock, Absolute);
            } break;
            case WE_GAINFOCUS:
            {
               isPaused = false;
               updateCursorStuff(window, game.sharedData.showCursor, game.sharedData.lockCursor, game.sharedData.relativeCursor);
            } break;
         }
      }

      DeltaTime time{};
      time.real.micros = logicClock.getDeltaMicros();
      time.real.seconds = logicClock.getDeltaSeconds();
      time.virt.micros = isPaused ? 0 : time.real.micros;
      time.virt.seconds = isPaused ? 0 : time.real.seconds;

      auto logic_ok = update_game(time, game, frameEvents, audioSystem, luaSystem.getStack(), graphicsSystem);

      if( !isPaused )
      {
         updateCursorStuff(window, game.sharedData.showCursor, game.sharedData.lockCursor, game.sharedData.relativeCursor);
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


