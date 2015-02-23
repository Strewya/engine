//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Rainbowland.h>
/******* C++ headers *******/
#include <algorithm>
#include <fstream>
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Games/Rainbowland/SessionPreparation.h>
#include <Games/Rainbowland/Session.h>
#include <Games/Rainbowland/SessionPerkMenu.h>
#include <Games/Rainbowland/SessionEnd.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
   void RainbowlandGame::shutdown()
   {
      DEBUG_INFO("---------------------------------");
      DEBUG_STATUS(true);
      m_window->lockCursor(false);
      m_window->showCursor(true);

      std::ofstream file("score");
      if (file.is_open())
      {
         file << m_highScore;
      }
      file.close();

      status &= m_textureCache.shutdown();
      status &= m_fontCache.shutdown();
      status &= m_luaSystem.shutdown();
      status &= m_inputSystem.shutdown();
      status &= m_renderQueue.shutdown();
      status &= m_audioSystem.shutdown();
      status &= m_graphicsSystem.shutdown();
      m_window->setFullscreen(false);
      if( !status )
      {
         DEBUG_INFO("\nShutdown has failed! Bugs...");
      }
   }

   bool RainbowlandGame::init(Window& window)
   {
      m_window = &window;

      window.resize(0, 0);
#ifndef DEPLOY
      window.openConsole(-1200, 0);
#endif

      m_isRunning =
         m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
         m_audioSystem.init() &&
         m_renderQueue.init(m_graphicsSystem) &&
         m_inputSystem.init(window) &&
         m_luaSystem.init() &&
         m_fontCache.init(m_textureCache) &&
         m_imageCache.init(m_textureCache) &&
         m_textureCache.init(m_graphicsSystem) &&
         true;

      window.setFullscreen(true);

      m_logicTimer.setTimeScale(1);
      m_renderTimer.setTimeScale(1);
      
      m_currentGameState = GS_Count; //invalid value until we are sure we can run

      if( m_isRunning )
      {
         initGame(*this);
      }

      DEBUG_INFO("---------------------------------");
      return m_isRunning;
   }

   bool RainbowlandGame::tick()
   {
      float fraction = 0;
      uint64_t unusedMicros = 0;
      uint64_t droppedTime = 0;
      static const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;

      uint32_t count, l;
      count = l = getLogicUpdateCount(m_logicTimer, microsPerFrame, fraction, unusedMicros, droppedTime);
      for( l; l--; )
      {
         tickLogic(microsPerFrame);
         if( !m_isRunning )
         {
            break;
         }
      }
      m_logicTimer.updateBy(droppedTime);

      uint64_t fullUpdateTime = m_logicTimer.getLastRealTimeMicros() + unusedMicros - m_renderTimer.getLastRealTimeMicros();
      if( count > 0 )
      {
         tickRender(fullUpdateTime);
      }
      return m_isRunning;
   }

   void RainbowlandGame::tickLogic(uint64_t updateTime)
   {
      if( m_nextGameState != m_currentGameState )
      {
         m_currentGameState = m_nextGameState;
      }

      m_logicTimer.updateBy(updateTime);

      m_inputSystem.update(m_logicTimer);
      
      m_audioSystem.update();
      
      switch( m_currentGameState )
      {
         case GS_SessionPreparation:
         {
            logic_sessionPreparation(*this);
         } break;

         case GS_Session:
         {
            logic_session(*this);
         } break;

         case GS_SessionPerkMenu:
         {
            logic_sessionPerkMenu(*this);
         } break;

         case GS_SessionEnd:
         {
            logic_sessionEnd(*this);
         } break;

         default:
         {
            //if we get here, that means there is something srsly wrong with the code
            assert(false && "HOLY SHIT SOMETHING WENT WRONG");
         } break;
      }
   }








   void RainbowlandGame::tickRender(uint64_t updateTime)
   {
      m_renderTimer.updateBy(updateTime);

      m_graphicsSystem.begin();

      switch( m_currentGameState )
      {
         case GS_SessionPreparation:
         {
            render_sessionPreparation(*this);
         } break;

         case GS_Session:
         {
            render_session(*this);
         } break;

         case GS_SessionPerkMenu:
         {
            render_session(*this);
            render_sessionPerkMenu(*this);
         } break;

         case GS_SessionEnd:
         {
            render_sessionEnd(*this);
         } break;

         default:
         {
            assert(false);
         } break;
      }

      m_graphicsSystem.present();
   }
}
