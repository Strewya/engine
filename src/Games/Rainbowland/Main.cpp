//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Main.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/LoadGameResources.h>
#include <Graphics/Camera.h>
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Vertex.h>
#include <Input/Keyboard.h>
#include <Util/Cache.h>
#include <Util/Color.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
#include <Window/Window.h>
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

   bool Game::init(Window& window)
   {
      CORE_INIT_START(Rainbowland);

      this->window = &window;

      window.resize(USE_MONITOR_RESOLUTION, USE_MONITOR_RESOLUTION);
#ifdef MURRAY
      window.move(window.getSizeX(), 0);
#endif
      window.showCursor(true);

      //window.setFullscreen(true);

      CORE_STATUS_AND(audioSystem.init());
      CORE_STATUS_AND(graphicsSystem.init(window));
      CORE_STATUS_AND(inputSystem.init(window));

      systems.audio = &audioSystem;
      systems.gfx = &graphicsSystem;
      systems.input = &inputSystem;

      assets = loadGameResources(systems);

      CORE_STATUS_AND(checkGameResourcesLoaded(assets));

      state.camera.setPosition({0, 0, -20});
      state.moveDirection.set(0, 0, 0);

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
         {Keyboard::ArrowUp, &state.moveDirection.y, +2},
         {Keyboard::ArrowDown, &state.moveDirection.y, -2},
         {Keyboard::ArrowLeft, &state.moveDirection.x, -2},
         {Keyboard::ArrowRight, &state.moveDirection.x, +2},
         {Keyboard::PageDown, &state.moveDirection.z, -2},
         {Keyboard::PageUp, &state.moveDirection.z, +2}
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
                        window->showCursor(!window->isCursorShown());
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
         }
      }

      state.camera.move({timer.getDeltaSeconds()*state.moveDirection.x,
                        timer.getDeltaSeconds()*state.moveDirection.y,
                        timer.getDeltaSeconds()*state.moveDirection.z});

      return running;
   }

   void Game::tickRender(uint32_t updateTime)
   {
      Mesh atlas = makeTexturedQuad({}, {1, 1}, assets.atlas, {0, 0}, {1, 1}, assets.mainVS, assets.mainPS);
      Mesh bgr = makeTexturedQuad({}, {1, 1}, assets.background, {0, 0}, {1, 1}, assets.mainVS, assets.mainPS);

      graphicsSystem.begin();

      graphicsSystem.setPerspectiveProjection();
      graphicsSystem.applyCamera(state.camera);

      //graphicsSystem.renderer.setCulling(true);
      //graphicsSystem.renderer.setTransparency(true);

      graphicsSystem.renderMesh(Transform{}, Color{}, bgr);

      graphicsSystem.present();
   }
}
