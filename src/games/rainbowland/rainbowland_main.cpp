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
#include "util/template/resource_cache_template.h"
#include "util/color.h"
#include "util/transform.h"
#include "util/utility.h"
/******* end headers *******/

namespace core
{
   bool Game::shutdown()
   {
      CORE_SHUTDOWN_START(Rainbowland);

      unloadGameResources(systems, assets);

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

      CORE_STATUS_AND(window.isValid());
      if( CORE_STATUS_OK )
      {
         window.resize(USE_MONITOR_RESOLUTION, USE_MONITOR_RESOLUTION);
#ifdef MURRAY
         window.move(window.getSizeX(), 0);
#endif
         window.showCursor(true);
         window.monitorDirectoryForChanges("resources");

         //window.setFullscreen(true);

         CORE_STATUS_AND(audioSystem.init());
         CORE_STATUS_AND(graphicsSystem.init(window));
         CORE_STATUS_AND(inputSystem.init(window));
         CORE_STATUS_AND(luaSystem.init());

         systems.audio = &audioSystem;
         systems.gfx = &graphicsSystem;
         systems.input = &inputSystem;
         systems.lua = &luaSystem;
      }

      if( CORE_STATUS_OK )
      {
         assets = loadGameResources(systems);

         CORE_STATUS_AND(checkGameResourcesLoaded(assets));
      }

      if( CORE_STATUS_OK )
      {
         state.camera.setPosition({0, 0, -20});

         initializeGameState(timer, state, systems, assets);
      }

      CORE_INIT_END(Rainbowland);
   }

   bool Game::tickLogic(uint32_t updateTime)
   {
      bool running = true;

      timer.updateBy(updateTime);

      inputSystem.update();

      auto events = inputSystem.getEvents();
      for( auto& e : events )
      {
         switch( e.type )
         {
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
         }
      }

      auto logic_ok = updateGameState(timer.getDeltaSeconds(), timer.getDeltaMicros(), state, systems, assets);

      return running && logic_ok;
   }

   void Game::tickRender(uint32_t updateTime)
   {
      graphicsSystem.begin();

      graphicsSystem.setPerspectiveProjection();
      graphicsSystem.applyCamera(state.camera);

      renderGameState(timer.getDeltaSeconds(), state, systems, assets);

      graphicsSystem.present();
   }
}


