//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
//none
/******* c++ headers *******/
#include <thread>
/******* extra headers *******/
#include "games/game_entry_point.h"
#include "window/window_include.h"
#include "window/window.h"
#include "window/window_class.h"
/******* end headers *******/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
   core::Window window("CoreEngine");

   int result = core::initializeWindow(window);

   if( result == core::WindowResult::OK )
   {
      std::thread logicThread(core::runGame, window.getProxy());

      while( window.getProxy().isRunning() )
      {
         window.update();
      }
      logicThread.join();
      result = window.getExitCode();
   }
   return result;
}
