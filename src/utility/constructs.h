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
   struct RingBuffer
   {
      u8* data;
      u32 readIndex;
      u32 writeIndex;
      u32 slots;
      u32 size;
      u32 align;

      void init(u8* memory, u32 slotSize, u32 slotCount, u32 alignment)
      {
         data = memory;
         readIndex = 0;
         writeIndex = 1;
         slots = slotCount;
         size = slotSize;
         align = alignment;
      }

   };
}