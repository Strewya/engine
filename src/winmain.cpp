//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
//none
/******* c++ headers *******/
#include <thread>
/******* extra headers *******/
#include "games/game_init.h"
#include "window/window_include.h"
#include "window/window.h"
#include "window/window_class.h"
/******* end headers *******/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
   Core::Window window("CoreEngine");

   int result = Core::initializeWindow(window);

   if( result == Core::WindowResult::OK )
   {
      std::thread logicThread(Core::runGame, std::ref(window));

      while( window.isRunning() )
      {
         window.update();
      }
      logicThread.join();
      result = window.getExitCode();
   }
   return result;
}
