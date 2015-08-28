#pragma once
/********************************************
*  contents:   entry point for game thread
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;
   struct Memory;

   void runGame(Memory memory, CommunicationBuffer* fromMain, CommunicationBuffer* toMain);
}