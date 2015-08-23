//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/utility.h"
/******* c++ headers *******/
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
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
      //                                   hh : mm : ss : ms        \0
      static const uint32_t timeTextSize = 2 + 1 + 2 + 1 + 2 + 1 + 3 + 1;
      char timeBuffer[timeTextSize] = {};
      //general time
      auto now = time(nullptr);
      auto msBuffer = timeBuffer + std::strftime(timeBuffer, timeTextSize, "%H:%M:%S.", localtime(&now));
      //milliseconds
      auto t = std::chrono::system_clock::now().time_since_epoch();
      auto s = std::chrono::duration_cast<std::chrono::seconds>(t);
      auto f = t - s;
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(f);
      sprintf(msBuffer, "%-3d", ms.count());

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

   void writeHeaderToLogStream(std::ostream& stream, const char* filepath, int line)
   {
      std::string file(filepath);
      file = file.substr(file.find_last_of("\\") + 1);
      std::string header = "[" + getCurrentTime() + "] " + file + "@" + std::to_string(line);
      const auto maxLen = 55;
      if( header.length() < maxLen )
      {
         header += std::string(maxLen - header.length(), '.');
      }
      stream << header;
   }
}
