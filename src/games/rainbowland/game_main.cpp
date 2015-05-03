//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/game_main.h"
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

namespace Core
{
   bool Game::shutdown()
   {
      CORE_SHUTDOWN_START(Rainbowland);

      unloadGameResources(systems, assets);

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

         systems.audio = &audioSystem;
         systems.gfx = &graphicsSystem;
         systems.input = &inputSystem;
      }

      if( CORE_STATUS_OK )
      {
         assets = loadGameResources(systems);

         CORE_STATUS_AND(checkGameResourcesLoaded(assets));
      }

      if( CORE_STATUS_OK )
      {
         state.camera.setPosition({0, 0, -20});
         state.cameraMoveDirection.set(0, 0, 0);

         initializeGameState(timer, state, systems, assets);
      }

      CORE_INIT_END(Rainbowland);
   }

   bool Game::tickLogic(uint32_t updateTime)
   {
      bool running = true;

      timer.updateBy(updateTime);

      inputSystem.update();

      struct SimpleKey
      {
         Keyboard::Key id;
         float* target;
         float change;
      };

      SimpleKey checkers[]
      {
         {Keyboard::ArrowUp, &state.cameraMoveDirection.y, +2},
         {Keyboard::ArrowDown, &state.cameraMoveDirection.y, -2},
         {Keyboard::ArrowLeft, &state.cameraMoveDirection.x, -2},
         {Keyboard::ArrowRight, &state.cameraMoveDirection.x, +2},
         {Keyboard::PageDown, &state.cameraMoveDirection.z, -2},
         {Keyboard::PageUp, &state.cameraMoveDirection.z, +2}
      };

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
                  case Keyboard::Space:
                  {
                     if( e.keyboard.firstTimeDown )
                     {
                        state.cameraMoveModifier = 10;
                     }
                     else if( e.keyboard.key.isDown == false )
                     {
                        state.cameraMoveModifier = 1;
                     }
                  } break;
               }

               for( auto& key : checkers )
               {
                  if( key.id == e.keyboard.key.id )
                  {
                     if( e.keyboard.firstTimeDown )
                     {
                        *key.target += key.change;
                     }
                     else if( e.keyboard.key.isDown == false )
                     {
                        *key.target -= key.change;
                     }
                  }
               }
            } break;
            
            case WE_FILECHANGE:
            {
               if(e.fileChange.action == FILE_MODIFIED )
               {
                  std::string file = e.fileChange.filename;
                  file = replaceAll(file, "\\", "/");
                  file = "../resources/" + file;
                  if( graphicsSystem.textures.isLoaded(file.c_str()) )
                  {
                     graphicsSystem.textures.reloadFromFile(file.c_str());
                  }
               }
            } break;
         }
      }

      state.camera.move({timer.getDeltaSeconds()*state.cameraMoveDirection.x*state.cameraMoveModifier,
                        timer.getDeltaSeconds()*state.cameraMoveDirection.y*state.cameraMoveModifier,
                        timer.getDeltaSeconds()*state.cameraMoveDirection.z*state.cameraMoveModifier});

      running = running && updateGameState(timer, state, systems, assets);

      return running;
   }

   void Game::tickRender(uint32_t updateTime)
   {
      graphicsSystem.begin();

      graphicsSystem.setPerspectiveProjection();
      graphicsSystem.applyCamera(state.camera);

      renderGameState(timer, state, systems, assets);

      graphicsSystem.present();
   }
}


