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
   core::Memory mainMemory{};
   core::Memory gameMemory{};

   auto communicationBufferSize = Bytes(sizeof(core::CommunicationBuffer));
   gameMemory.size = Megabytes(256);
   mainMemory.size = gameMemory.size + communicationBufferSize * 2;

#ifndef DEPLOY
   auto baseAddress = (LPVOID)Gigabytes((u64)1);
#else
   auto baseAddress = (LPVOID)0;
#endif

   mainMemory.ptr = (LPVOID)VirtualAlloc(baseAddress, (SIZE_T)mainMemory.size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
   i32 result = core::WindowResult::OK;
   if( mainMemory.ptr )
   {
      core::CommunicationBuffer* toGame = (core::CommunicationBuffer*)mainMemory.ptr;
      core::CommunicationBuffer* fromGame = toGame + 1;
      gameMemory.ptr = (u8*)mainMemory.ptr + communicationBufferSize * 2;

      toGame->init();
      fromGame->init();

      core::Window window("CoreEngine");

      result = core::initializeWindow(window);
      if( result == core::WindowResult::OK )
      {
         std::thread logicThread(core::runGame, gameMemory, toGame, fromGame);
         core::WinMsg msg;
         msg.type = core::WinMsgType::WindowHandle;
         msg.handle = (u64)window.getWindowHandle();
         toGame->writeEvent(msg);

         while( window.processWin32Messages(toGame) )  //INFINITE LOOP MESSAGE PUMP
         {
            // #think
            //maybe read messages from the game to the window? it might be a nice way of cross thread communication...
            //could be used as a means for the game to ask the window to close because it is dead...
            //and could be used to ask the window stuff from the game without actually doing it through a pointer/proxy...
            window.processCommands(fromGame, toGame);
         }

         msg.type = core::WinMsgType::Close;
         toGame->writeEvent(msg);

         logicThread.join();
         result = window.getExitCode();
      }
      else
      {
         MessageBox(nullptr, "Cannot create window", "Startup error", MB_OK);
      }
   }
   else
   {
      MessageBox(nullptr, "Not enough memory to run this game. Sorry :(", "Startup error", MB_OK);
      result = core::WindowResult::InsufficientMemory;
   }

   return result;
}
