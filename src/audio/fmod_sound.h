#pragma once
/********************************************
*  contents:   fmod sound resource object
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "audio/fmod_include.h"
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct FmodSound
   {
      FMOD::Sound* _sound;

      bool loaded()
      {
         return _sound != nullptr;
      }

      bool unloaded()
      {
         return !loaded();
      }
   };
}
