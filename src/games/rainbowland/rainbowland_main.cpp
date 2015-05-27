//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/rainbowland_main.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/load_game_resources.h"
#include "graphics/camera.h"
#include "graphics/mesh/mesh.h"
#include "graphics/vertex.h"
#include "input/keyboard.h"
#include "util/color.h"
#include "util/transform.h"
#include "util/utility.h"
#include "util/template/resource_cache_template.h"
#include "util/time/clock.h"
/******* end headers *******/

namespace core
{
   bool Game::shutdown()
   {
      CORE_SHUTDOWN_START(Rainbowland);

      unloadGameResources(systems, assets);

      CORE_STATUS_AND(fontSystem.shutdown());
      CORE_STATUS_AND(luaSystem.shutdown());
      CORE_STATUS_AND(inputSystem.shutdown());
      CORE_STATUS_AND(graphicsSystem.shutdown());
      CORE_STATUS_AND(audioSystem.shutdown());

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
         window.move(window.getSizeX(), 0);
//#endif
         window.showCursor(true);
         window.monitorDirectoryForChanges("resources");

         //window.setFullscreen(true);

         CORE_STATUS_AND(audioSystem.init());
         CORE_STATUS_AND(graphicsSystem.init(window));
         CORE_STATUS_AND(inputSystem.init(window));
         CORE_STATUS_AND(luaSystem.init());
         CORE_STATUS_AND(fontSystem.init(graphicsSystem.textures));

         systems.audio = &audioSystem;
         systems.gfx = &graphicsSystem;
         systems.input = &inputSystem;
         systems.lua = &luaSystem;
         systems.font = &fontSystem;
      }

      if( CORE_STATUS_OK )
      {
         assets = loadGameResources(systems);

         CORE_STATUS_AND(checkGameResourcesLoaded(assets));
      }

      if( CORE_STATUS_OK )
      {
         state.camera.setPosition({0, 0, -20});

         game_init(state, systems, assets);
      }

      CORE_INIT_END(Rainbowland);
   }

   bool Game::tickLogic(const Clock& logicClock)
   {
      bool running = true;

      inputSystem.gatherInputForCurrentFrame(logicClock.getCurrentMicros());
      
      const auto& events = inputSystem.getEvents();
      for( auto& e : events )
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
               if(e.fileChange.action == FILE_MODIFIED )
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

            case WE_LOSTFOCUS:
            {
               isPaused = true;
            } break;

            case WE_GAINFOCUS:
            {
               isPaused = false;
            } break;
         }
      }

      auto logic_ok = true;
      if( !isPaused )
      {
         logic_ok = game_update(logicClock.getDeltaSeconds(), logicClock.getDeltaMicros(), state, systems, assets);
      }

      luaSystem.collectGarbage();

      return running && logic_ok;
   }

   void Game::tickRender(const Clock& renderClock)
   {
      graphicsSystem.begin();

      systems.gfx->setOrthographicProjection();

      if(isPaused )
      {
         auto mesh = fontSystem.makeTextMesh("Paused", state.fontDesc, {1, 1}, {Center, Middle});
         systems.gfx->renderMesh({vec2f{0, 400}}, {}, mesh);
      }
      else
      {
         auto mesh = fontSystem.makeTextMesh("Running", state.fontDesc, {1, 1}, {Center, Middle});
         systems.gfx->renderMesh({vec2f{0, 400}}, {}, mesh);

         game_render(renderClock.getDeltaSeconds(), state, systems, assets);
      }

      graphicsSystem.present();
   }
}


