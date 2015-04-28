#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
#include <audio/fmod_include.h>
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
