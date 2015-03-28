#pragma once
/********************************************
*  class:   Mouse
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <unordered_map>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Mouse
   {
   private:
      typedef std::unordered_map<uint32_t, std::string> Map;
      static Map m_map;

   public:
      static void Init();
      static int32_t Code(const std::string& name);
      static std::string Name(uint32_t code);

      static const int32_t Unknown = -1;
      enum Key : uint8_t
      {
         LeftButton,
         RightButton,
         MiddleButton,
         XButton1,
         XButton2,

         KeyCount
      };
   };
}