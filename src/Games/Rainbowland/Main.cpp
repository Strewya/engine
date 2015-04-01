//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Main.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Color.h>
#include <Util/Utility.h>
#include <Util/Vec2.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
   void Game::shutdown()
   {
      CORE_INFO("----- shutdown start -----");
      CORE_STATUS(m_graphicsSystem.shutdown());

      if( CORE_STATUS_NOK )
      {
         CORE_INFO("\nShutdown has failed! Bugs...");
      }
   }

   bool Game::init(Window& window)
   {
      m_window = &window;

      window.resize(MONITOR_RESOLUTION, MONITOR_RESOLUTION);
#ifndef DEPLOY
      window.openConsole(-1200, 0);
#endif
      //window.setFullscreen(true);

      CORE_STATUS(m_graphicsSystem.init(window, {1200, 900}));

      CORE_INFO("----- init done -----");
      return CORE_STATUS_OK;
   }

   bool Game::tickLogic(uint64_t updateTime)
   {
      bool running = true;

      return running;
   }

   void Game::tickRender(uint64_t updateTime)
   {
      struct Dude
      {
         struct Transform
         {
            Vec2f position;
            float scale;
            float rotationRad;
         } transform;
         struct VisualPart
         {
            Color color;
            struct MeshId
            {
               int id;
            } meshId;
         } visualPart;
      } guy;


      m_graphicsSystem.begin();
      /*
      auto& mesh = m_meshStore.getMesh(guy.visualPart.meshId);
      m_gfxProxy.renderMesh(guy.transform, guy.color, mesh);
         m_graphicsSystem.setTexture(mesh.textureId);
         m_graphicsSystem.setShader(mesh.shaderId);
         m_graphicsSystem.setBlendMode(mesh.blendModeId);
         m_graphicsSystem.renderMesh(guy.transform, guy.color, mesh.vertices, mesh.indices);
      */
      m_graphicsSystem.present();
   }
}
