#pragma once
/********************************************
*  contents:   entry point for game thread
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;
   struct MainAllocator;

   void runGame(MainAllocator& allocator,
                CommunicationBuffer* fromMain, CommunicationBuffer* toMain,
                u64 windowHandle, u32 windowWidth, u32 windowHeight);
}