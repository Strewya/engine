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

      CORE_INIT_END(Rainbowland);
   }

   bool Game::tickLogic(uint64_t updateTime)
   {
      bool running = true;

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
                     if(e.keyboard.firstTimeDown )
                        window->showCursor(!window->isCursorShown());
                  } break;
               }
            } break;
         }
      }

      return running;
   }

   void Game::tickRender(uint64_t updateTime)
   {
      Mesh atlas = makeTexturedQuad({}, {1, 1}, assets.atlas, {0, 0}, {1, 1}, assets.mainVS, assets.mainPS);
      Mesh bgr = makeTexturedQuad({}, {1, 1}, assets.background, {0, 0}, {1, 1}, assets.mainVS, assets.mainPS);

      Camera camera;
      camera.setPosition({0, 0, -10});
      
      graphicsSystem.begin();

      graphicsSystem.setPerspectiveProjection();
      graphicsSystem.applyCamera(camera);
      
      //graphicsSystem.renderer.setCulling(true);
      //graphicsSystem.renderer.setTransparency(true);

      graphicsSystem.renderMesh(Transform{}, Color{}, bgr);
      
      graphicsSystem.present();
   }
}
