#pragma once
/********************************************
*  contents:   Utility defines and functions
*  usage:
********************************************/
/******* c++ headers *******/
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include "util/memory_sizes.h"
/******* end header inclusion *******/

namespace core
{
#ifndef CORE_DEPLOY

   inline void debugPrint()
   {
      std::cout << std::endl;
   }
   template<typename T, typename... Args> void debugPrint(T t, Args... args)
   {
      std::cout << t; debugPrint(args...);
   }

#define CORE_INFO(...) debugPrint(__VA_ARGS__)

#else
#define CORE_INFO(...) (void)0
#endif
   extern uint32_t g_indent;

#define UP_INDENT ++g_indent
#define DOWN_INDENT --g_indent

#define STR(type) #type

#define CORE_PHASE_INIT "init"
#define CORE_PHASE_SHUTDOWN "shutdown"
#define CORE_PHASE_LOG(obj, phase, text) CORE_INFO( std::string(g_indent, ' '), obj, " "phase" ", text)
#define CORE_START_PHASE(obj, phase) UP_INDENT; CORE_PHASE_LOG(obj, phase, "start"); bool status = true
#define CORE_END_PHASE(obj, phase) CORE_PHASE_LOG(obj, phase, status ? "OK" : "FAILED"); DOWN_INDENT; return status


#define CORE_INIT_START_STR(c) CORE_START_PHASE(c, CORE_PHASE_INIT)
#define CORE_INIT_END_STR(c) CORE_END_PHASE(c, CORE_PHASE_INIT)

#define CORE_INIT_START(c) CORE_INIT_START_STR(#c)
#define CORE_INIT_END(c) CORE_INIT_END_STR(#c)

#define CORE_SHUTDOWN_START_STR(c) CORE_START_PHASE(c, CORE_PHASE_SHUTDOWN)
#define CORE_SHUTDOWN_END_STR(c) CORE_END_PHASE(c, CORE_PHASE_SHUTDOWN)

#define CORE_SHUTDOWN_START(c) CORE_SHUTDOWN_START_STR(#c)
#define CORE_SHUTDOWN_END(c) CORE_SHUTDOWN_END_STR(#c)

#define CORE_STATUS_OK (status == true)
#define CORE_STATUS_NOK (status == false)
#define CORE_STATUS_SET(x) status = (x)
#define CORE_STATUS_AND(x) status = (x) && status
#define CORE_STATUS_OR(x) status = (x) || status

#define CORE_RESOURCE(x) "../resources/"x

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
