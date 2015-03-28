#pragma once
/********************************************
*  class:   Debug utilities
*  usage:
********************************************/
/******* C++ headers *******/
#include <algorithm>
#include <iostream>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{

#define CORE_STATUS(x) bool status = (x)
#define CORE_STATUS_AND(x) status = (x) && status
#define CORE_STATUS_OR(x) status = (x) || status
#define CORE_STATUS_NOK (status == false)
#define CORE_STATUS_OK (status == true)
#define CORE_INIT(c) CORE_INFO( #c" init ", status ? "OK" : "FAIL"); return status
#define CORE_SHUTDOWN(c) CORE_INFO( #c" shutdown ", status ? "OK" : "FAIL"); return status


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
