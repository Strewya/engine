//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/ChaosGarden/Game.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Window/Window.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   void Game::shutdown()
   {
      CORE_INFO("----- shutdown start -----");
      bool status = true;
      status = m_graphicsSystem.shutdown() && status;
      
      if( !status )
      {
         CORE_INFO("\nShutdown has failed! Bugs...");
      }
   }

   bool Game::init(Window& window)
   {
      m_window = &window;

      window.resize(0, 0);
#ifndef DEPLOY
      window.openConsole(-1200, 0);
#endif
      //window.setFullscreen(true);

      CORE_STATUS(true);
      CORE_STATUS_AND(m_graphicsSystem.init(window));
      CORE_STATUS_AND(m_textureStore.init(m_graphicsSystem));
      CORE_STATUS_AND(m_fontStore.init(m_textureStore));

      CORE_INFO("----- init done -----");
      return true;
   }

   bool Game::tickLogic(uint64_t updateTime)
   {
      return true;
   }

   void Game::tickRender(uint64_t updateTime)
   {
      
   }
}
