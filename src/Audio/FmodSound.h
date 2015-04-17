#pragma once
/********************************************
*  contents:   FmodSound
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
#include <Audio/FmodInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class FmodSound
   {
   public:
      FMOD::Sound* sound;

      bool loaded()
      {
         return sound != nullptr;
      }

      bool unloaded()
      {
         return !loaded();
      }
   };
}
