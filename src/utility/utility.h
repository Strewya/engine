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

   // #todo think about what to do with this
   enum class AssertLevel
   {
      Fatal,
      Notification,
   };

#define CORE_LOG(...) writeLog(__FILE__, __LINE__, __VA_ARGS__)
#define CORE_ASSERT(level, condition, ...) do { if(!(condition)) { CORE_LOG(__VA_ARGS__); auto l = level; if(l == AssertLevel::Fatal) { *(int*)0 = 42; } } } while(!(condition))

#ifndef CORE_DEPLOY

#define CORE_LOG_DEBUG(...) CORE_LOG(__VA_ARGS__)
#define CORE_ASSERT_DEBUG(level, condition, ...) CORE_ASSERT(level, condition, __VA_ARGS__)

#else

#define CORE_LOG_DEBUG(...) (void)0
#define CORE_ASSERT_DEBUG(...) (void)0

#endif


#define STR(type) #type

#define CORE_STATUS bool status = true
#define CORE_STATUS_OK (status == true)
#define CORE_STATUS_NOK (status == false)
#define CORE_STATUS_SET(x) status = (x)
#define CORE_STATUS_AND(x) status = status && (x)

#define CORE_PHASE_INIT "init"
#define CORE_PHASE_SHUTDOWN "shutdown"
#define CORE_PHASE_OBJECT(obj) const char* core_phase_object = #obj
#define CORE_PHASE_LOG(phase, text) CORE_LOG_DEBUG( core_phase_object, " "phase" ", text)
#define CORE_START_PHASE(phase) CORE_PHASE_LOG(phase, "start"); CORE_STATUS
#define CORE_END_PHASE(phase) CORE_PHASE_LOG(phase, status ? "OK" : "FAILED"); return status

// #deprecated remove this asap after implementing fixed memory
#define CORE_INIT_START(c) CORE_PHASE_OBJECT(c); CORE_START_PHASE(CORE_PHASE_INIT)
#define CORE_INIT_END CORE_END_PHASE(CORE_PHASE_INIT)

#define CORE_SHUTDOWN_START(c) CORE_PHASE_OBJECT(c); CORE_START_PHASE(CORE_PHASE_SHUTDOWN)
#define CORE_SHUTDOWN_END CORE_END_PHASE(CORE_PHASE_SHUTDOWN)

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

   inline std::string replaceOne(std::string str, std::string from, std::string to)
   {
      size_t start_pos = str.find(from);
      if( start_pos != std::string::npos )
      {
         str.replace(start_pos, from.length(), to);
      }
      return str;
   }

   inline std::string replaceAll(std::string str, std::string from, std::string to)
   {
      if( !from.empty() )
      {
         size_t start_pos = 0;
         while( (start_pos = str.find(from, start_pos)) != std::string::npos )
         {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
         }
      }
      return str;
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
