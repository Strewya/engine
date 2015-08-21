//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/utility.h"
/******* c++ headers *******/
#include <chrono>
#include <ctime>
#include <fstream>
/******* extra headers *******/
#include "util/time/clock.h"
/******* end headers *******/

namespace core
{
   static std::ofstream gLogFileStream;

   static std::string getCurrentDate()
   {
      //                               YYYY_mm_dd         \0
      static const uint32_t dateSize = 4 + 1 + 2 + 1 + 2 + 1;
      char dateBuffer[dateSize] = {};

      auto now = time(nullptr);
      std::strftime(dateBuffer, dateSize, "%Y_%m_%d", localtime(&now));
      return dateBuffer;
   }

   static std::string getCurrentTime()
   {
      //                                   hh : mm : ss : msms        \0
      static const uint32_t timeTextSize = 2 + 1 + 2 + 1 + 2 + 1 + 4 + 1;
      char timeBuffer[timeTextSize] = {};

      auto now = time(nullptr);
      std::strftime(timeBuffer, timeTextSize, "%H:%M:%S.0000", localtime(&now));
      // #todo figure out how to do milliseconds
      return timeBuffer;
   }

   static std::string getFilename()
   {
      std::string filename = "log_" + getCurrentDate() + ".log";
      return filename;
   }

   std::ostream& getLogFileStream()
   {
      if( !gLogFileStream.is_open() )
      {
         gLogFileStream.open(getFilename().c_str(), std::ios_base::app);
         gLogFileStream << std::endl << std::endl << "Execution start" << std::endl;
      }
      return gLogFileStream;
   }

   void writeHeaderToLogStream(std::ostream& stream, const char* file, int line)
   {
      stream << "[" << getCurrentTime() << "]" << file << ":" << line << " ... ";
   }
}
