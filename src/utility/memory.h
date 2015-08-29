#pragma once
/********************************************
*  contents:   Utility defines and functions
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

#define Bytes(n) (n)
#define Kilobytes(n) ((n)*1024)
#define Megabytes(n) (Kilobytes(n)*1024)
#define Gigabytes(n) (Megabytes(n)*1024)
#define Terabytes(n) (Gigabytes(n)*1024)

namespace core
{
   struct Memory
   {
      u64 size;
      void* ptr;
   };
}