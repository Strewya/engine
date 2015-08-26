//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
//none
/******* c++ headers *******/
#include <array>
#include <thread>
/******* extra headers *******/
#include "games/game_entry_point.h"
#include "util/memory.h"
#include "util/types.h"
#include "util/utility.h"
#include "util/time/clock.h"
#include "util/communication_buffer.h"
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
      core::Memory mainMemory{};
      core::Memory gameMemory{};

      u32 communicationBufferSize = Bytes(sizeof(core::CommunicationBuffer));
      gameMemory.size = Megabytes(256);
      mainMemory.size = gameMemory.size + communicationBufferSize;



#ifndef DEPLOY
      LPVOID baseAddress = (LPVOID)Gigabytes((u64)1);
#else
      LPVOID baseAddress = 0;
#endif
      mainMemory.ptr = (LPVOID)VirtualAlloc(baseAddress, (SIZE_T)mainMemory.size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if( mainMemory.ptr )
      {
         core::CommunicationBuffer* communication = (core::CommunicationBuffer*)mainMemory.ptr;
         gameMemory.ptr = (u8*)mainMemory.ptr + communicationBufferSize;

         //std::thread logicThread(core::runGame, window.getProxy(), gameMemory, communication);

         while( window.processWin32Messages(communication) ) {} //INFINITE LOOP MESSAGE PUMP

         core::WindowEvent kill;
         kill.type = core::WindowEventType::WE_LASTEVENT;
         kill.timestamp = core::Clock::getRealTimeMicros();

         //logicThread.join();
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
