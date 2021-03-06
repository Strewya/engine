#pragma once
/********************************************
*  contents:   Utility defines and functions
*  usage:
********************************************/
/******* c++ headers *******/
#include <algorithm>
#include <iostream>
#include <string>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Memory;

   void initializeFileStream(Memory& m, u32 bufferSize);

   std::ostream& getLogFileStream();


   inline std::ostream& newLine(std::ostream& stream)
   {
      stream << "\n";
      return stream;
   }

   inline std::ostream& logLine(std::ostream& stream)
   {
      stream << "\n        ";
      return stream;
   }

   void writeHeaderToLogStream(std::ostream& stream, const char* file, int line);

   inline void writeContentToLogStream(std::ostream& stream)
   {
      //endl flushes the stream, which is what i want here.
      stream << std::endl;
   }

   template<typename T, typename... Args> void writeContentToLogStream(std::ostream& stream, T t, Args... args)
   {
      stream << t; writeContentToLogStream(stream, args...);
   }
   template<typename... Args> inline void writeLog(const char* file, int line, Args... args)
   {
      auto& logStream = getLogFileStream();
#ifndef DEPLOY
      writeHeaderToLogStream(logStream, file, line);
#endif
      writeContentToLogStream(logStream, args...);
   }

#define CORE_CRASH *(int*)0 = 42
#define CORE_LOG(...) writeLog(__FILE__, __LINE__, __VA_ARGS__)

#define CORE_ASSERT_ERR(condition, ...) do { if(!(condition)) { CORE_LOG("ERROR: ", __VA_ARGS__); CORE_CRASH; } } while(0)
#define CORE_ASSERT_WRN(condition, ...) do { if(!(condition)) { CORE_LOG("WARNING: ", __VA_ARGS__); } } while(0)

#ifndef DEPLOY

#define CORE_LOG_DEBUG(...) CORE_LOG(__VA_ARGS__)
#define CORE_ASSERT_DBGERR(condition, ...) CORE_ASSERT_ERR(condition, __VA_ARGS__)
#define CORE_ASSERT_DBGWRN(condition, ...) CORE_ASSERT_WRN(condition, __VA_ARGS__)

#else

#define CORE_LOG_DEBUG(...) (void)0
#define CORE_ASSERT_DBGERR(condition, ...) (void)0
#define CORE_ASSERT_DBGWRN(condition, ...) (void)0

#endif

#define CORE_RESOURCE(x) "resources/"x

#define CORE_PI 3.14159265358979f
#define Deg2Rad(deg) ((deg)*CORE_PI/180.0f)
#define Rad2Deg(rad) ((rad)*180.0f/CORE_PI)


   template<typename T>
   void wrap(T& value, T low, T high)
   {
      auto d = high - low;
      while( value < low )
      {
         value += d;
      }
      while( value > high )
      {
         value -= d;
      }
   }

   template<typename T>
   void clamp(T& value, T low, T high)
   {
      if( value < low )
      {
         value = low;
      }
      else if( value > high )
      {
         value = high;
      }
   }

   template<typename T>
   T max(T a, T b)
   {
      return (a < b) ? (b) : (a);
   }

   template<typename T, typename ...Args>
   T max(T a, Args... rest)
   {
      auto m = max(rest...);
      return max(a, m);
   }

   template<typename T>
   T min(T a, T b)
   {
      return (a < b) ? (a) : (b);
   }

   template<typename T, typename ...Args>
   T min(T a, Args... rest)
   {
      auto m = min(rest...);
      return min(a, m);
   }

   template<typename T>
   T abs(T v)
   {
      if( v < 0 )
      {
         v = -v;
      }
      return v;
   }

   template<typename C, typename F> size_t filterFind(const C& container, F filter)
   {
      return std::distance(std::begin(container), std::find_if(std::begin(container), std::end(container), filter));
   }

   template<typename C, typename T> size_t valueFind(const C& container, T value)
   {
      return std::distance(std::begin(container), std::find(std::begin(container), std::end(container), value));
   }

}
