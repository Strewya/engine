//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
//none
/******* C++ headers *******/
#include <thread>
/******* extra headers *******/
#include <Games/GameInit.h>
#include <Window/myWindows.h>
#include <Window/Window.h>
#include <Window/WindowClass.h>
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
