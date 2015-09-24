//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
//none
/******* c++ headers *******/
#include <thread>
/******* extra headers *******/
#include "games/game_entry_point.h"
#include "utility/memory.h"
#include "utility/types.h"
#include "utility/utility.h"
#include "utility/time/clock.h"
#include "utility/communication_buffer.h"
#include "window/window_include.h"
#include "window/window.h"
#include "window/window_class.h"
#include "window/window_message.h"
/******* end headers *******/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
   core::Window window("CoreEngine");
   
   i32 result = core::initializeWindow(window);
   if( result == core::WindowResult::OK )
   {
#ifndef DEPLOY
      LPVOID baseAddress = (LPVOID)TeraBytes(1);
#else
      LPVOID baseAddress = (LPVOID)0;
#endif
      SIZE_T size = GigaBytes(1);
      LPVOID address = VirtualAlloc(baseAddress, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

      if( address )
      {
         using core::writeLog;
         core::MainAllocator mainAllocator;
         mainAllocator.init(address, size);

         initializeFileStream(mainAllocator, KiloBytes(128));

         core::CommunicationBuffer* toGame = mainAllocator.allocate<core::CommunicationBuffer>();
         toGame->init(mainAllocator, 2048);

         core::CommunicationBuffer* fromGame = mainAllocator.allocate<core::CommunicationBuffer>();
         fromGame->init(mainAllocator, 128);

         CORE_LOG("Status after allocating communication buffers:", core::logLine, mainAllocator);

         std::thread logicThread(core::runGame, std::ref(mainAllocator), toGame, fromGame, (u64)window.getWindowHandle(), window.getSizeX(), window.getSizeY());

         while( window.processWin32Messages(toGame) )  //INFINITE LOOP MESSAGE PUMP
         {
            // #think
            //maybe read messages from the game to the window? it might be a nice way of cross thread communication...
            //could be used as a means for the game to ask the window to close because it is dead...
            //and could be used to ask the window stuff from the game without actually doing it through a pointer/proxy...
            window.processCommands(fromGame, toGame);
         }

         core::WinMsg msg{};
         msg.type = core::WinMsgType::Close;
         toGame->writeEvent(msg);

         logicThread.join();
         result = window.getExitCode();
         CORE_LOG_DEBUG(mainAllocator);
      }
      else
      {
         MessageBox(nullptr, "Not enough memory to run this game. Sorry :(", "Startup error", MB_OK);
         result = core::WindowResult::InsufficientMemory;
      }
   }
   else
   {
      MessageBox(nullptr, "Cannot create window", "Startup error", MB_OK);
   }

   return result;
}
