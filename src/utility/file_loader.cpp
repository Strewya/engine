//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/file_loader.h"
/******* c++ headers *******/
#include <fstream>
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   LoadFileResult loadFile(const char* filename, u8* buffer, u32& bufferSize)
   {
      std::ifstream file{filename, std::ifstream::in | std::ifstream::binary};
      if( file.good() )
      {
         file.seekg(0, std::ios::end);
         auto fileSize = size_t(file.tellg());
         if( bufferSize >= fileSize )
         {
            file.seekg(0, std::ios::beg);
            file.read((char*)buffer, fileSize);
            file.close();
            bufferSize = (u32)fileSize;
            return LoadFileResult::OK;
         }
         return LoadFileResult::BufferTooSmall;
      }
      return LoadFileResult::FileOpenError;
   }
}
