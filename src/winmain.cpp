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
         core::Memory memory{address, size};

         auto before = __rdtsc();
         core::initializeFileStream(memory, KiloBytes(128));
         CORE_LOG(core::newLine, core::newLine, core::logLine, "Execution start", core::newLine);

         core::CommunicationBuffer* toGame = core::emplace<core::CommunicationBuffer>(memory);
         toGame->init(memory, 2048);

         core::CommunicationBuffer* fromGame = core::emplace<core::CommunicationBuffer>(memory);
         fromGame->init(memory, 128);
         auto after = __rdtsc();

         CORE_LOG("Initializing log and comm buffers took ", (after - before), " cycles");
         CORE_LOG("Status after allocating communication buffers:", core::logLine, memory);
         
         std::thread logicThread(core::runGame, memory, toGame, fromGame, (u64)window.getWindowHandle(), window.getSizeX(), window.getSizeY());

         while( window.processWin32Messages(toGame) )  //INFINITE LOOP MESSAGE PUMP
         {
            window.processCommands(fromGame, toGame);
         }

         core::WinMsg msg{};
         msg.type = core::WinMsgType::Close;
         toGame->writeEvent(msg);

         logicThread.join();
         result = window.getExitCode();
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
