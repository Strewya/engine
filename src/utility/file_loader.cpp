//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/file_loader.h"
/******* c++ headers *******/
#include <fstream>
/******* extra headers *******/
#include "utility/memory.h"
/******* end headers *******/

namespace core
{
   LoadedFile loadFile(const char* filename, StackAllocator& a)
   {
      LoadedFile result{nullptr, 0};
      std::ifstream file{filename, std::ifstream::in | std::ifstream::binary};
      if( file.good() )
      {
         file.seekg(0, std::ios::end);
         result.size = size_t(file.tellg());
         result.memory = allocate(a, result.size, 1);
         if( result.memory )
         {
            file.seekg(0, std::ios::beg);
            file.read((char*)result.memory, result.size);
         }
         file.close();
      }
      return result;
   }
}
