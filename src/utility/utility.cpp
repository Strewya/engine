//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/utility.h"
/******* c++ headers *******/
#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
/******* extra headers *******/
#include "utility/time/clock.h"
/******* end headers *******/

namespace core
{
   core_internal u32 fillCurrentDate(char* buffer, u32 size)
   {
      auto now = time(nullptr);
      u32 written = (u32)std::strftime(buffer, size, "%Y_%m_%d", localtime(&now));
      return written;
   }

   core_internal u32 fillCurrentTime(char* buffer, u32 size)
   {
      //general time
      auto now = time(nullptr);
      u32 written = (u32)std::strftime(buffer, size, "%H:%M:%S.", localtime(&now));
      size -= written;
      buffer += written;
      //milliseconds
      auto t = std::chrono::system_clock::now().time_since_epoch();
      auto s = std::chrono::duration_cast<std::chrono::seconds>(t);
      auto f = t - s;
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(f);
      written += _snprintf(buffer, size, "%-3d", ms.count());
      return written;
   }

   core_internal void getFilename(char* buffer, u32 size)
   {
      u32 written = _snprintf(buffer, size, "log_");
      size -= written;
      buffer += written;
      written = fillCurrentDate(buffer, size);
      size -= written;
      buffer += written;
      written = _snprintf(buffer, size, ".log");
      size -= written;
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, size >= 0);
   }

   std::ostream& getLogFileStream()
   {
      core_local_persist std::ofstream gLogFileStream;

      if( !gLogFileStream.is_open() )
      {
         char filename[19] = {};
         getFilename(filename, 19);

         gLogFileStream.open(filename, std::ios_base::app);
         gLogFileStream << newLine << newLine << logLine << "Execution start" << newLine;
      }
      return gLogFileStream;
   }

   void writeHeaderToLogStream(std::ostream& stream, const char* filepath, int line)
   {
      char buffer[13] = {};
      u32 written = fillCurrentTime(buffer, 13);
      stream << "\033[1;31m[" << buffer;
      while( written++ < 12 )
      {
         stream << '0';
      }
      stream << "] ";

      u32 filenameLength = 0;
      const char* finder = filepath + strlen(filepath) - 1;
      while( *finder != '\\' && *finder != '/' && finder > filepath )
      {
         --finder;
         ++filenameLength;
      }
      ++finder;
      stream << finder << " @ " << line << "\033[0m" << logLine;
   }
}
