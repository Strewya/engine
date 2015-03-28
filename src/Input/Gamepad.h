#pragma once
/********************************************
*  class:   Gamepad
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
   class Gamepad
   {
   private:
      typedef std::unordered_map<uint32_t, std::string> Map;
      static Map m_map;

   public:
      static void Init();
      static int32_t Code(std::string name);
      static std::string Name(uint32_t code);

      static const int32_t Unknown = -1;
      enum Key : uint8_t
      {
         DPadUp,
         DPadDown,
         DPadLeft,
         DPadRight,
         LeftButton,
         TopButton,
         RightButton,
         BottomButton,
         LeftThumb,
         RightThumb,
         LeftShoulder,
         RightShoulder,
         Start,
         Back,
         LeftStick,
         RightStick,
         LeftTrigger,
         RightTrigger,

         KeyCount
      };
   };
}