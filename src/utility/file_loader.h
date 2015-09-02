#pragma once
/********************************************
*  contents:   function to load a file into byte array
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   enum class LoadFileResult
   {
      FileOpenError,
      BufferTooSmall,
      OK,
   };

   LoadFileResult loadFile(const char* filename, u8* buffer, u32 bufferSize);
}
