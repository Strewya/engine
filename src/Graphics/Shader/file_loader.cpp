//headers should be ordered alphabetically!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <graphics/shader/file_loader.h>
/******* c++ headers *******/
#include <fstream>
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   bool loadFile(const std::string& filename, std::vector<char>& buffer)
   {
      size_t shaderSize = 0;
      std::ifstream shaderFile{filename, std::ifstream::in | std::ifstream::binary};
      if( shaderFile.good() )
      {
         shaderFile.seekg(0, std::ios::end);
         shaderSize = size_t(shaderFile.tellg());
         buffer.clear();
         buffer.resize(shaderSize);
         shaderFile.seekg(0, std::ios::beg);
         shaderFile.read(&buffer[0], shaderSize);
         shaderFile.close();
         return true;
      }
      return false;
   }
}
