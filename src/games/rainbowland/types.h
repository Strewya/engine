#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct BlockAllocator
   {
      Memory memory;
      void* nextMemory;
   };
}
