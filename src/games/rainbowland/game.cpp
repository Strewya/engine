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
   bool Game::shutdown()
   {
      CORE_SHUTDOWN_START(Rainbowland);

      unloadGameResources(game.assets, audioSystem.sounds, graphicsSystem.pixelShaders, graphicsSystem.vertexShaders, graphicsSystem.textures);

      CORE_STATUS_AND(fontSystem.shutdown());
      CORE_STATUS_AND(luaSystem.shutdown());
      CORE_STATUS_AND(inputSystem.shutdown());
      CORE_STATUS_AND(graphicsSystem.shutdown());
      CORE_STATUS_AND(audioSystem.shutdown());

      window.makeMouseRelative(false);
      window.showCursor(true);
      window.lockCursor(false);

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
         window.showCursor(false);
         window.lockCursor(true);
         window.makeMouseRelative(true);

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

         CORE_STATUS_AND(game_init(game, audioSystem, graphicsSystem, luaSystem.getStack()));
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
            // #temp this case is temporary until i get a normal menu type thing
            // which will handle shutting the game down
            case WE_KEYBOARDKEY:
            {
               switch( e.keyboard.key.id )
               {
                  case Keyboard::Escape:
                  {
                     running = false;
                  } break;
                  case Keyboard::F1:
                  {
                     if( e.keyboard.firstTimeDown )
                     {
                        window.showCursor(!window.isCursorShown());
                     }
                  } break;
               }
            } break;

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
               window.showCursor(true);
               window.lockCursor(false);
               window.makeMouseRelative(false);
            } break;
            case WE_GAINFOCUS:
            {
               isPaused = false;
               window.showCursor(game.constants.showCursor);
               window.lockCursor(game.constants.lockCursor);
               window.makeMouseRelative(game.constants.relativeCursor);
            } break;
         }
      }

      Time time{};
      time.deltaMicrosReal = logicClock.getDeltaMicros();
      time.deltaTimeReal = logicClock.getDeltaSeconds();
      time.deltaMicrosVirt = isPaused ? 0 : time.deltaMicrosReal;
      time.deltaTimeVirt = isPaused ? 0 : time.deltaTimeReal;

      auto logic_ok = game_update(time, game, frameEvents, audioSystem, luaSystem.getStack(), graphicsSystem);

      luaSystem.collectGarbage();
      audioSystem.update();

      return running && logic_ok;
   }

   void Game::tickRender(const Clock& clock)
   {
      graphicsSystem.begin();

      Time time{};
      time.deltaMicrosReal = clock.getDeltaMicros();
      time.deltaTimeReal = clock.getDeltaSeconds();
      time.deltaMicrosVirt = isPaused ? 0 : time.deltaMicrosReal;
      time.deltaTimeVirt = isPaused ? 0 : time.deltaTimeVirt;

      game_render(time, game, graphicsSystem, fontSystem);

      graphicsSystem.present();
   }
}


